from vosk import Model, KaldiRecognizer
import wave
import json
import pyodbc
from difflib import SequenceMatcher

# --------------------- Partie 1 : Reconnaissance vocale ---------------------

# Ouvre le fichier WAV
wf = wave.open("C:/Users/Oussema Agrebi/voice_final.wav", "rb")

# Vérifie les paramètres audio
if wf.getnchannels() != 1 or wf.getsampwidth() != 2 or wf.getframerate() != 16000:
    print("❌ Le fichier audio doit être WAV mono (1 canal), 16kHz, 16 bits.")
    exit(1)

# Charge le modèle Vosk
model = Model("C:\\Users\\Oussema Agrebi\\OneDrive\\Bureau\\interface_employe\\resources\\vosk-model-small-fr-0.22")

# Prépare la reconnaissance
recognizer = KaldiRecognizer(model, wf.getframerate())

# Lis le fichier audio et accumule le texte
texte_final = ""

while True:
    data = wf.readframes(4000)
    if len(data) == 0:
        break
    if recognizer.AcceptWaveform(data):
        result = json.loads(recognizer.Result())
        texte_final += result.get("text", "") + " "

# Résultat final
result = json.loads(recognizer.FinalResult())
texte_final += result.get("text", "")
texte_final = texte_final.strip()

print("🗣️ Texte reconnu :", texte_final)

# --------------------- Partie 2 : Connexion à la base ODBC ---------------------

def recuperer_noms_employes_odbc():
    try:
        conn = pyodbc.connect(
    r'DRIVER={SQL Server Native Client 11.0};SERVER=localhost;DATABASE=projet2A;UID=oussema;PWD=oussema'
)



        cursor = conn.cursor()
        cursor.execute("SELECT NOM, PRENOM FROM employes")
        noms = cursor.fetchall()
        conn.close()
        return noms
    except Exception as e:
        print("❌ Erreur de connexion à la base :", e)
        return []

# --------------------- Partie 3 : Identification de l'employé (version améliorée) ---------------------

def similar(a, b):
    return SequenceMatcher(None, a.lower(), b.lower()).ratio()

def identifier_employe_depuis_texte(texte, liste_noms, seuil=0.7):
    texte = texte.lower()
    mots = texte.split()

    for nom, prenom in liste_noms:
        for mot in mots:
            if similar(mot, nom) > seuil or similar(mot, prenom) > seuil:
                return f"{prenom} {nom}"
    return None

# Utilisation
noms_employes = recuperer_noms_employes_odbc()
employe_trouve = identifier_employe_depuis_texte(texte_final, noms_employes)

if employe_trouve:
    print(f"✅ Employé reconnu : {employe_trouve}")
else:
    print("❌ Aucun employé correspondant trouvé dans la base.")
