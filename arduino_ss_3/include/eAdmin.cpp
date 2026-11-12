/*
Composant : eAdmin
Support   : ESP32
Version/Auteur/Date : V2 BAPTISTE 05-11-2026
Réalisé par BAPTISTE à partir de eAdmin.h (DaK 20260923)
Fonction :
Implémentation de eAdmin.h
Page Admin.html : liste des UIDs, ajout, suppression, lien vers les logs
Le serveur web de la page est dans ce fichier : le reste du programme n'a pas à le connaître
*/
#include <Arduino.h>
#include <string>
#include <WebServer.h>
#include <LittleFS.h>
#include "pinOut.h"
#include "eAdmin.h"

using std::string;

#include "eBDD.h"
#include "eLog.h"

#define PORT_ADMIN     80
#define UID_LEN_MIN    8
#define UID_LEN_MAX    20

bool     pinPrete       = false;    /*!< vrai quand pinADMIN est configurée */
bool     fenetreOuverte = false;    /*!< vrai tant que le badge administrateur est valable */
uint32_t debutFenetre   = 0;        /*!< millis() au passage du badge */
uint32_t dureeFenetre   = 0;        /*!< durée de validité du badge en ms */
bool     serveurLance   = false;    /*!< vrai quand le serveur web est démarré */
WebServer serveurAdmin( PORT_ADMIN );   /*!< le serveur web de la page Admin */


// UID valide : hexadécimal, longueur paire entre 8 et 20. Le met en majuscules
bool uidValide( string& uid )
{
    bool valide = true;
    int n = uid.length();

    if (n < UID_LEN_MIN || n > UID_LEN_MAX || n % 2 != 0)
    {
        valide = false;
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            if (!isxdigit(uid[i])) valide = false;
            uid[i] = toupper(uid[i]);
        }
    }
    return valide;
}

string pageRefus()
{
    string page = "<!DOCTYPE html>";
    page += "<html lang='fr'>";
    page += "<head>";
    page += "<meta charset='UTF-8'>";
    page += "<meta http-equiv='refresh' content='5'>";
    page += "<title>Accès refusé</title>";
    page += "</head>";
    page += "<body style='font-family:sans-serif; max-width:640px; margin:2rem auto'>";
    page += "<h1>Accès refusé</h1>";
    page += "<p>Présentez le badge administrateur devant la borne. La page se rafraîchit toute seule.</p>";
    page += "</body>";
    page += "</html>";
    return page;
}


string construirePage( const string& action, const string& uid );
uint32_t AdminRestant();

// Le même test que AdminActive, mais sans toucher au serveur (appelé depuis les pages)
bool accesAutorise()
{
    if (fenetreOuverte && (uint32_t)(millis() - debutFenetre) >= dureeFenetre)
    {
        fenetreOuverte = false;
    }
    return fenetreOuverte || digitalRead(pinADMIN) == LOW;
}

// Le navigateur demande /Admin.html : on lui envoie la page (action et uid viennent de la page)
void envoyerPage()
{
    string action = serveurAdmin.arg("action").c_str();
    string uid    = serveurAdmin.arg("uid").c_str();
    serveurAdmin.send(200, "text/html", construirePage(action, uid).c_str());
}

// Le navigateur demande /log.csv : on envoie le fichier de logs, seulement si l'accès est autorisé
void envoyerLogs()
{
    if (!accesAutorise())
    {
        serveurAdmin.send(403, "text/plain", "Acces refuse");
    }
    else
    {
        File f = LittleFS.open("/log.csv", "r");
        if (!f)
        {
            serveurAdmin.send(404, "text/plain", "Pas de log");
        }
        else
        {
            serveurAdmin.sendHeader("Content-Disposition", "attachment; filename=log.csv");
            serveurAdmin.streamFile(f, "text/csv");
            f.close();
        }
    }
}

// Démarre le serveur web une seule fois : on lui dit quelle fonction répond à quelle adresse
void lancerServeur()
{
    if (!serveurLance)
    {
        serveurAdmin.on("/Admin.html", envoyerPage);
        serveurAdmin.on("/log.csv", envoyerLogs);
        serveurAdmin.begin();
        serveurLance = true;
    }
}

// Un badge vient d'être lu : si c'est l'admin, on autorise la page pendant "secondes" secondes
// On retient seulement l'heure de départ et la durée, rien d'autre
bool AdminUID( bool isAdmin, uint16_t secondes )
{
    if (!isAdmin || secondes == 0)
    {
        fenetreOuverte = false;
    }
    else
    {
        fenetreOuverte = true;
        debutFenetre   = millis();
        dureeFenetre   = secondes * 1000UL;
    }
    return AdminActive();
}

// L'accès à la page est-il autorisé ? (badge admin récent, ou pinADMIN à LOW)
// A appeler dans loop() : c'est aussi ici que le serveur web répond au navigateur
bool AdminActive()
{
    if (!pinPrete)
    {
        pinMode(pinADMIN, INPUT_PULLUP);
        pinPrete = true;
    }
    if (fenetreOuverte && (uint32_t)(millis() - debutFenetre) >= dureeFenetre)
    {
        fenetreOuverte = false;
    }
    lancerServeur();
    serveurAdmin.handleClient();        // répond aux demandes du navigateur, s'il y en a
    return fenetreOuverte || digitalRead(pinADMIN) == LOW;
}

// Secondes avant fermeture de la page : on prend la durée totale moins le temps déjà écoulé
uint32_t AdminRestant()
{
    uint32_t restant = 0;

    if (accesAutorise())
    {
        if (fenetreOuverte)
        {
            uint32_t ecoule = millis() - debutFenetre;      // ms depuis le badge
            restant = (dureeFenetre - ecoule) / 1000;        // en secondes
        }
        else
        {
            restant = 0xFFFFFFFF;                            // ouverte par pinADMIN, pas de limite
        }
    }
    return restant;
}

// Construit la page HTML (ou la page "Accès refusé"), et applique l'action demandée (ajout, suppression)
string construirePage( const string& action, const string& uid )
{
    string page;

    if (!accesAutorise())
    {
        page = pageRefus();
    }
    else
    {
        // action demandée par la page : ajout, ajoutAdmin ou supprime
        string uidAction = uid;
        if (uidValide(uidAction))
        {
            if (action == "ajout" || action == "ajoutAdmin")
            {
                addUID(uidAction.c_str(), action == "ajoutAdmin");
                espLog(("ajout UID " + uidAction).c_str());
            }
            else if (action == "supprime")
            {
                delUID(uidAction.c_str());
                espLog(("suppression UID " + uidAction).c_str());
            }
        }

        page = "<!DOCTYPE html>";
        page += "<html lang='fr'>";

        page += "<head>";
        page += "<meta charset='UTF-8'>";
        page += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
        page += "<title>Admin barrière</title>";
        page += "<style>";
        page += "body { font-family: sans-serif; max-width: 640px; margin: 1rem auto; padding: 0 1rem; }";
        page += "table { width: 100%; border-collapse: collapse; }";
        page += "td, th { border-bottom: 1px solid #ccc; padding: 0.4rem; text-align: left; }";
        page += "</style>";
        page += "</head>";

        page += "<body>";
        page += "<h1>Administration de la barrière</h1>";
        page += "<p>";
        page += std::to_string(UIDs(false)); page += " badges (";
        page += std::to_string(UIDs(true));  page += " admin), ";
        page += std::to_string(free());      page += " places libres";
        page += "</p>";
        if (fenetreOuverte)
        {
            page += "<p>Fermeture de la page dans " + std::to_string(AdminRestant()) + " s</p>";
        }
        page += "<p><a href='/log.csv'>Télécharger les logs</a></p>";

        // liste des UID : liste() renvoie [{"UID":"04A1B2C3","admin":true},{"UID":"...","admin":false}]
        page += "<h2>Badges enregistrés</h2>";
        page += "<table>";
        page += "<tr><th>UID</th><th>Rôle</th><th></th></tr>";
        string json = liste();
        size_t position = 0;
        while ((position = json.find("\"UID\":\"", position)) != string::npos)
        {
            position += 7;
            size_t fin = json.find("\"", position);
            string uidListe = json.substr(position, fin - position);
            bool admin = (json.compare(json.find("\"admin\":", fin) + 8, 4, "true") == 0);

            page += "<tr>";
            page += "<td>" + uidListe + "</td>";
            page += string("<td>") + (admin ? "administrateur" : "utilisateur") + "</td>";
            page += "<td><a href='Admin.html?action=supprime&uid=" + uidListe + "'>Supprimer</a></td>";
            page += "</tr>";
        }
        page += "</table>";

        page += "<h2>Ajouter un badge</h2>";
        page += "<form action='Admin.html' method='get'>";
        page += "<input name='uid' maxlength='20' placeholder='UID hexa, ex. 04A1B2C3' required> ";
        page += "<button type='submit' name='action' value='ajout'>Ajouter</button> ";
        page += "<button type='submit' name='action' value='ajoutAdmin'>Ajouter administrateur</button>";
        page += "</form>";

        page += "</body>";
        page += "</html>";
    }
    return page;
}