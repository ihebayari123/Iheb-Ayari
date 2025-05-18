# Système de Gestion de Véhicules Électriques – Simulation

Ce projet consiste à simuler un système de gestion de véhicules électriques en utilisant **MicroC PRO for PIC** et **Proteus ISIS**, basé sur le microcontrôleur **PIC16F877A**.

## 📌 Objectifs du projet
- Contrôler et surveiller un système de gestion de véhicules électriques.
- Simuler des capteurs, affichages et modules de contrôle.
- Tester la logique embarquée avec un microcontrôleur PIC.

## 🧰 Outils utilisés
- **MicroC PRO for PIC** – Pour la programmation du microcontrôleur.
- **Proteus ISIS** – Pour la simulation du circuit électronique.
- **PIC16F877A** – Microcontrôleur utilisé dans le projet.

## 📁 Structure du projet
/projet-vehicules-electriques/
│
├── code/ # Fichiers .c et .h pour le microcontrôleur
├── simulation/ # Fichiers .DSN (ISIS Proteus)

## ▶️ Fonctionnalités simulées
- Allumage automatique des véhicules (LEDs)
- Affichage du niveau de batterie (LCD/7 segments)
- Système de recharge
- Détection de surchauffe ou panne (via capteurs simulés)

## ⚙️ Microcontrôleur utilisé
- **PIC16F877A**
  - 40 broches
  - 8 canaux ADC
  - Compatible avec de nombreux capteurs et modules

## 🧪 Comment tester
1. Ouvrez le fichier `.pdsprj` dans **Proteus ISIS**.
2. Chargez le fichier `.hex` généré depuis **MicroC** dans le microcontrôleur.
3. Démarrez la simulation et observez le comportement du système.

