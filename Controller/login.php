<?php
// Inclure le fichier config.php
require_once '../config.php'; // Vérifiez bien le chemin

session_start();

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Récupérer les informations saisies par l'utilisateur
    $email = $_POST['email'] ?? '';
    $password = $_POST['password'] ?? '';

    try {
        // Connexion à la base de données
        $pdo = config::getConnexion();

        // Préparer une requête pour vérifier les informations de connexion
        $stmt = $pdo->prepare("SELECT * FROM users WHERE email = :email");
        $stmt->bindParam(':email', $email, PDO::PARAM_STR);
        $stmt->execute();
        $user = $stmt->fetch(PDO::FETCH_ASSOC);

        if ($user && password_verify($password, $user['password'])) {
            // Connexion réussie : créer une session
            $_SESSION['user_id'] = $user['id'];
            $_SESSION['user_email'] = $user['email'];
            $_SESSION['user_name'] = $user['Name'];
            $_SESSION['user_role'] = $user['role'];

            // Redirection selon le rôle
            if ($user['role'] === 'Admin') {
                header("Location: ../View/Backoffice/forum.php");
            } else {
                header("Location: ../View/Frontoffice/profiluser.php");
            }
            exit();
        } else {
            // Connexion échouée : Redirection avec message d'erreur
            header("Location: ../View/Frontoffice/account.php?error=1");
            exit();
        }
    } catch (Exception $e) {
        die('Erreur lors de la connexion : ' . $e->getMessage());
    }
} else {
    echo "Méthode de requête non valide.";
}
?>
