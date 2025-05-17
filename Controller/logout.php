<?php
session_start();

// Vérifier si l'utilisateur est connecté en front office
if (isset($_SESSION['user_id'])) {
    // Supprimer uniquement les informations de la front office
    unset($_SESSION['user_id']);
    unset($_SESSION['user_name']);
    unset($_SESSION['user_surname']);
    unset($_SESSION['user_email']);
    unset($_SESSION['user_role']);
    
    // Rediriger vers la page de login de la front office
    header("Location: ../View/Frontoffice/signin.php");
    exit();
} else {
    // Si l'utilisateur n'est pas connecté, rediriger vers la page d'accueil
    header("Location: ../View/Frontoffice/signin.php");
    exit();
}
