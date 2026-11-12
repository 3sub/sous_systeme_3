/*
Composant : eLogic
Support   : ESP32
Version/Auteur/Date : V2.0 Ton 20261112
*/
#include <Arduino.h>
#include <string.h>

#include "eAdmin.h"
#include "eBDD.h"
#include "eFileSystem.h"
#include "eGenCode.h"
#include "eLog.h"
#include "eLogic.h"
#include "eSerial.h"
#include "eWebCode.h"

#define RX_MEGA      16
#define TX_MEGA      17
#define LEN_CODE      4
#define TIMEOUT_CODE (5 * 60)

static char uidCourant[21] = "";   // badge en attente de son code


// Cherche l'UID dans la liste JSON de eBDD. Renvoie vrai s'il existe, et dit s'il est admin.
static bool chercherUID(const char* uid, bool* admin)
{
    static char json[4096];
    char cle[32];

    if (!liste(json, sizeof(json))) return false;

    snprintf(cle, sizeof(cle), "\"%s\"", uid);
    char* p = strstr(json, cle);
    if (p == NULL) return false;

    *admin = false;
    char* v = strstr(p, "\"admin\"");
    if (v != NULL) v = strchr(v, ':');
    if (v != NULL)
    {
        v++;
        while (*v == ' ') v++;
        *admin = (strncmp(v, "true", 4) == 0);
    }
    return true;
}

// Le MEGA a lu un badge
static void badgeLu(const char* uid)
{
    bool admin = false;

    if (!chercherUID(uid, &admin))
    {
        espLog("badge inconnu");
        uidCourant[0] = '\0';
        WrDENIED();
        return;
    }

    AdminUID(admin);

    uint32_t code = genCode(uid, LEN_CODE);
    if (code == 0)
    {
        espLog("erreur generation code");
        WrDENIED();
        return;
    }

    char texte[12];
    snprintf(texte, sizeof(texte), "%lu", (unsigned long)code);
    Affiche2FA(texte, TIMEOUT_CODE);

    strncpy(uidCourant, uid, sizeof(uidCourant) - 1);
    uidCourant[sizeof(uidCourant) - 1] = '\0';
    espLog("badge connu, code affiche");
}

// Le MEGA envoie le code tapé au clavier
static void codeTape(const char* texte)
{
    if (uidCourant[0] == '\0')
    {
        espLog("code tape sans badge");
        WrDENIED();
        return;
    }

    uint32_t kbd = strtoul(texte, NULL, 10);

    if (compareCode(kbd, TIMEOUT_CODE))
    {
        espLog("acces autorise");
        WrGRANTED();
    }
    else
    {
        espLog("code faux ou expire");
        WrDENIED();
    }
    uidCourant[0] = '\0';
}

static void traiterMessage(const char* msg)
{
    if (strncmp(msg, "UID:", 4) == 0)        badgeLu(msg + 4);
    else if (strncmp(msg, "CODE:", 5) == 0)  codeTape(msg + 5);
    else                                     espLog("message MEGA inconnu");
}


// à appeler dans setup()
void SetupLogic()
{
    Serial.begin(115200);
    beginSerialMEGA(Serial2, 9600, RX_MEGA, TX_MEGA);
    WebInit();
    espLog("demarrage ESP32");
}

// à appeler dans loop()
void LoopLogic()
{
    AdminActive();

    if (Serial2.available())
    {
        delay(30);  // laisse arriver tout le message (readMEGA ne garde pas les messages coupés)
        char msg[32];
        if (readMEGA(msg, sizeof(msg)) > 0)
        {
            traiterMessage(msg);
        }
    }
}