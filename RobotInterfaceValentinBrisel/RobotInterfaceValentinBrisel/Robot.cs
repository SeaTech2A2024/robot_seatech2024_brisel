using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RobotInterfaceValentinBrisel
{
    
    public class Robot
    {
        public string receivedText = "";
        public Queue<byte> byteListReceived = new Queue<byte>();
        public float distanceTelemetreDroit;
        public float distanceTelemetreCentre;
        public float distanceTelemetreGauche;
        public byte vitesse_gauche;
        public byte vitesse_droit;


        public Robot()
        {
        }

        public enum LED
        {
            led_rouge,
            led_bleue,
            led_blanc
        }

        public enum StateRobot
        {
            STATE_ATTENTE = 0,
            STATE_ATTENTE_EN_COURS = 1,
            STATE_AVANCE = 2,
            STATE_AVANCE_EN_COURS = 3,
            STATE_TOURNE_GAUCHE = 4,
            STATE_TOURNE_GAUCHE_EN_COURS = 5,
            STATE_TOURNE_DROITE = 6,
            STATE_TOURNE_DROITE_EN_COURS = 7,
            STATE_TOURNE_SUR_PLACE_GAUCHE = 8,
            STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS = 9,
            STATE_TOURNE_SUR_PLACE_DROITE = 10,
            STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS = 11,
            STATE_ARRET = 12,
            STATE_ARRET_EN_COURS = 13,
            STATE_RECULE = 14,
            STATE_RECULE_EN_COURS = 15
        }
    }

}
