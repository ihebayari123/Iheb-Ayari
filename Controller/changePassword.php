<?php
session_start();
require_once '../config.php';
require_once 'userController.php';

if (!isset($_SESSION['user_id'])) {
    echo "Utilisateur non connecté.";
    header("Location: ../view/Frontoffice/account.php"); // Redirigez si non connecté
    exit();
}

// Vérifiez si le formulaire est soumis
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Vérifiez que tous les champs sont présents dans $_POST
    if (!isset($_POST['current_password'], $_POST['new_password'], $_POST['confirm_password'])) {
        echo "Tous les champs sont requis.";
        exit();
    }

    $userId = $_SESSION['user_id'];
    $currentPassword = $_POST['current_password'];
    $newPassword = $_POST['new_password'];
    $confirmPassword = $_POST['confirm_password'];

    // Vérifiez si le nouveau mot de passe et la confirmation correspondent
    if ($newPassword !== $confirmPassword) {
        echo "Le nouveau mot de passe et la confirmation ne correspondent pas.";
        exit();
    }

    // Vérifiez la longueur du nouveau mot de passe (exemple : au moins 6 caractères)
    if (strlen($newPassword) < 6) {
        echo "Le nouveau mot de passe doit comporter au moins 6 caractères.";
        exit();
    }

    // Obtenez la connexion à la base de données
    $db = config::getConnexion();

    // Créez une instance du contrôleur
    $controller = new UserController($db);

    // Récupérez les informations de l'utilisateur
    $user = $controller->getuser($userId);

    if (!$user) {
        echo "Utilisateur non trouvé.";
        exit();
    }

    // Vérifiez que l'ancien mot de passe est correct
    if (!password_verify($currentPassword, $user['password'])) {
        echo "Mot de passe actuel incorrect.";
        exit();
    }

    // Hachez le nouveau mot de passe
    $hashedPassword = password_hash($newPassword, PASSWORD_BCRYPT);

    // Mettez à jour le mot de passe dans la base de données
    $updateQuery = $db->prepare("UPDATE users SET password = :password WHERE id = :id");

    try {
        $updateQuery->execute(['password' => $hashedPassword, 'id' => $userId]);
        echo "Mot de passe modifié avec succès.";
    } catch (Exception $e) {
        echo "Erreur lors de la mise à jour du mot de passe : " . $e->getMessage();
    }

    // Redirigez vers le profil après la mise à jour
    header("Location: ../View/Frontoffice/profiluser.php");
    exit(); // Assurez-vous que l'exécution du script s'arrête après la redirection
} else {
    echo "Méthode invalide.";
}
?>
