<?php
include '../../Controller/userController.php';

// Vérifier si le formulaire est soumis
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $name = $_POST['Name'];
    $surname = $_POST['surname'];
    $email = $_POST['Email'];
    $password = $_POST['password'];
    $role = $_POST['role'];

    // Hacher le mot de passe avant de l'ajouter
    $hashedPassword = password_hash($password, PASSWORD_DEFAULT);

    // Ajouter l'utilisateur via le contrôleur
    $userC = new userController();
    $userC->addUser($name, $surname, $email, $hashedPassword, $role);

    // Rediriger en fonction du rôle
    if ($role === 'User') {
        header('Location: http://localhost/ProjectForum/View/Frontoffice/index.php');
    } elseif ($role === 'Admin') {
        header('Location: userList.php?message=Admin ajouté avec succès');
    }
    exit;
}
?>
