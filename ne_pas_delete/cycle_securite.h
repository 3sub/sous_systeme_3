/**
 * @file cycle_securite.h
 * @brief Composant de gestion du cycle et des sécurités - SS3 (Barrière Sécurisée)
 * @target Arduino MEGA 2560 (C / C++)
 * @version 2.0
 */

#ifndef CYCLE_SECURITE_H
#define CYCLE_SECURITE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                               ÉNUMÉRATIONS                                 */
/* ========================================================================== */

/**
 * @brief États de la machine à états principale du cycle barrière.
 */
typedef enum {
    STATE_INIT = 0,         /**< Initialisation et calibration de la position */
    STATE_FERMEE,           /**< Barrière fermée, attente véhicule */
    STATE_ATTENTE_AUTH,     /**< Véhicule présent (IN), attente décision authentification */
    STATE_OUVERTURE,        /**< Moteur en cours de montée */
    STATE_OUVERTE,          /**< Barrière ouverte, temporisation en cours */
    STATE_FERMETURE,        /**< Moteur en cours de descente */
    STATE_DEFAUT            /**< État d'urgence/sécurité : moteur bloqué */
} CycleState_t;

/**
 * @brief Commandes moteurs adressées au Pont en H.
 */
typedef enum {
    MOTOR_CMD_STOP = 0,     /**< Moteur désactivé / Freinage */
    MOTOR_CMD_OPEN,         /**< Commande de montée (sens horlogique/direct) */
    MOTOR_CMD_CLOSE         /**< Commande de descente (sens anti-horlogique/inverse) */
} MotorCmd_t;

/**
 * @brief Types d'anomalies de sécurité recensées.
 */
typedef enum {
    FAULT_NONE = 0,              /**< Aucune anomalie */
    FAULT_TIMEOUT_OPENING,       /**< Dépassement de temps lors de la montée */
    FAULT_TIMEOUT_CLOSING,       /**< Dépassement de temps lors de la descente */
    FAULT_INCOHERENT_SENSORS,    /**< Deux fins de course actifs simultanément */
    FAULT_EMERGENCY_TRIGGERED    /**< Déclenchement d'un arrêt d'urgence explicite */
} FaultType_t;

/**
 * @brief Ordres reçus sur la liaison série Rx/Tx.
 */
typedef enum {
    CMD_SERIAL_NONE = 0,    /**< Aucun ordre reçu */
    CMD_SERIAL_IN,          /**< Message "IN\n" (Véhicule à l'entrée) */
    CMD_SERIAL_OUT,         /**< Message "OUT\n" (Véhicule à la sortie) */
    CMD_SERIAL_OK,          /**< Message "OK\n" (Authentification réussie) */
    CMD_SERIAL_NOK,         /**< Message "NoK\n" (Authentification échouée) */
    CMD_SERIAL_TIMEOUT      /**< Message "TimeOut\n" (Authentification expirée) */
} SerialCmd_t;

/* ========================================================================== */
/*                               STRUCTURES                                   */
/* ========================================================================== */

/**
 * @brief Structure de configuration matérielle et temporelle du module.
 */
typedef struct {
    // Broches d'E/S Arduino MEGA
    uint8_t pin_fin_haut;          /**< Pin entrée fin de course haut */
    uint8_t pin_fin_bas;           /**< Pin entrée fin de course bas */
    uint8_t pin_moteur_in1;        /**< Pin sortie relais 1 Pont en H */
    uint8_t pin_moteur_in2;        /**< Pin sortie relais 2 Pont en H */
    uint8_t pin_moteur_ena;        /**< Pin sortie Enable / PWM Pont en H */
    
    // Temporisations (en ms)
    uint32_t tempo_mouvement_max_ms; /**< Temps max autorisé pour ouvrir/fermer (ex: 8000 ms) */
    uint32_t tempo_ouverte_ms;       /**< Durée de maintien en position haute (ex: 5000 ms) */
    
    // Polarité des fins de course (true si niveau HIGH = actif, false si LOW = actif)
    bool capteurs_active_high;
} CycleConfig_t;

/* ========================================================================== */
/*                           PROTOTYPES DE FONCTIONS                          */
/* ========================================================================== */

/**
 * @brief Initialise le composant logiciel, configure les broches d'E/S et applique l'état INIT.
 * @param config Pointeur vers la structure de configuration
 */
void cycle_init(const CycleConfig_t *config);

/**
 * @brief Fonction principale à appeler en boucle non-bloquante (loop Arduino).
 *        Met à jour la machine à états, gère les temporisations et lit les capteurs.
 */
void cycle_process(void);

/**
 * @brief Traite un ordre reçu depuis la liaison série Rx/Tx ASCII.
 * @param cmd Commande énumérée correspondant à la chaîne répertoriée (OK, NoK, TimeOut, IN, OUT)
 */
void cycle_handle_serial_cmd(SerialCmd_t cmd);

/**
 * @brief Parse directement une chaîne ASCII brute terminée par '\n' reçue sur la liaison série.
 * @param ascii_str Chaîne de caractères reçue (ex: "OK\n", "IN\n")
 */
void cycle_parse_ascii_message(const char *ascii_str);

/**
 * @brief Force l'arrêt immédiat du moteur et passe le système en état de DÉFAUT.
 * @param cause La raison/anomalie déclenchant l'arrêt
 */
void cycle_trigger_emergency_stop(FaultType_t cause);

/**
 * @brief Réinitialise l'état de défaut après correction de l'anomalie.
 */
void cycle_reset_fault(void);

/* ========================================================================== */
/*                             FONCTIONS GETTERS                              */
/* ========================================================================== */

/**
 * @brief Récupère l'état courant de la machine à états.
 * @return CycleState_t État actuel
 */
CycleState_t cycle_get_state(void);

/**
 * @brief Récupère le type de défaut actif s'il y a lieu.
 * @return FaultType_t Type de défaut
 */
FaultType_t cycle_get_fault(void);

/**
 * @brief Vérifie si le moteur est actuellement en rotation.
 * @return true Si le moteur tourne (ouverture ou fermeture), false sinon.
 */
bool cycle_is_motor_running(void);

#ifdef __cplusplus
}
#endif

#endif /* CYCLE_SECURITE_H */