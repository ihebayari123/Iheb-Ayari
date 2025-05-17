<?php
include(__DIR__ . '/../config.php');
include(__DIR__ . '/../Model/user.php');

class userController {

    // Liste tous les utilisateurs
    public function listuser() {
        $sql = "SELECT * FROM users";
        $db = config::getConnexion();

        try {
            $list = $db->query($sql);
            return $list;
        } catch (Exception $err) {
            echo $err->getMessage();
        }
    }

    // Ajouter un nouvel utilisateur
    public function addUser($name, $surname, $email, $password, $role) {
        $db = config::getConnexion();
        try {
            $query = $db->prepare(
                'INSERT INTO  users (Name, surname, Email, password, role) 
                VALUES (:name, :surname, :email, :password, :role)'
            );
            $query->execute([
                'name' => $name,
                'surname' => $surname,
                'email' => $email,
                'password' => $password,
                'role' => $role
            ]);
        } catch (Exception $e) {
            die('Erreur : ' . $e->getMessage());
        }
    }
    
    

    // Supprimer un utilisateur
    public function deleteuser($id) {
        $sql = "DELETE FROM users WHERE id = :id";
        $db = config::getConnexion();

        try {
            $query = $db->prepare($sql);
            $query->bindValue(':id', $id, PDO::PARAM_INT); // Sécurisation de l'ID
            $query->execute();
            return $query->rowCount(); // Retourne le nombre de lignes affectées
        } catch (Exception $e) {
            echo "Error: " . $e->getMessage();
            return false; // Retourne false en cas d'erreur
        }
    }

    // Récupérer un utilisateur spécifique
    public function getuser($id) {
        $sql = "SELECT * FROM users WHERE id = :id";
        $db = config::getConnexion();
    
        try {
            $query = $db->prepare($sql);
            $query->execute(['id' => $id]);
            $user = $query->fetch();
            return $user; // Inclut toutes les colonnes, y compris 'role'
        } catch (Exception $e) {
            echo $e->getMessage();
        }
    }
    

    // Mettre à jour un utilisateur
    public function updateuser($id, $name, $surname, $email, $password, $role) {
        $sql = "UPDATE users SET Name = :name, surname = :surname, Email = :email, password = :password, role = :role WHERE id = :id";
        $db = config::getConnexion();
    
        try {
            $query = $db->prepare($sql);
            $query->execute([
                ':name' => $name,
                ':surname' => $surname,
                ':email' => $email,
                ':password' => $password,
                ':role' => $role,
                ':id' => $id
            ]);
        } catch (Exception $e) {
            echo $e->getMessage();
        }
    }
    public function searchUsersByName($name) {
    $db = config::getConnexion();
    try {
        $query = $db->prepare('SELECT * FROM user.users WHERE Name LIKE :name');
        $query->execute(['name' => '%' . $name . '%']);
        return $query->fetchAll();
    } catch (PDOException $e) {
        echo 'Erreur : ' . $e->getMessage();
        return [];
    }
}
public function listUsersSorted($sortBy = null, $sortOrder = 'ASC') {
    $sql = "SELECT * FROM users";
    $db = config::getConnexion();  // Ajout de la connexion à la base de données
    
    // Ajouter la clause ORDER BY si un critère de tri est spécifié
    if ($sortBy) {
        $sql .= " ORDER BY " . $sortBy . " " . $sortOrder;
    }

    try {
        $stmt = $db->prepare($sql);
        $stmt->execute();
        return $stmt->fetchAll(PDO::FETCH_ASSOC);
    } catch (Exception $e) {
        echo "Error: " . $e->getMessage();
        return [];
    }
}
public function changePassword() {
    if ($_SERVER['REQUEST_METHOD'] === 'POST') {
        session_start();
        $userId = $_SESSION['user_id']; // Assurez-vous que l'utilisateur est connecté
        $currentPassword = $_POST['current_password'];
        $newPassword = $_POST['new_password'];
        $confirmPassword = $_POST['confirm_password'];

        // Récupérer le mot de passe actuel depuis la base de données
        $query = $this->db->prepare("SELECT password FROM users WHERE id = :id");
        $query->execute(['id' => $userId]);
        $user = $query->fetch();

        if (!$user || !password_verify($currentPassword, $user['password'])) {
            echo "Mot de passe actuel incorrect.";
            return;
        }

        if ($newPassword !== $confirmPassword) {
            echo "Le nouveau mot de passe et la confirmation ne correspondent pas.";
            return;
        }

        // Mettre à jour le mot de passe
        $hashedPassword = password_hash($newPassword, PASSWORD_BCRYPT);
        $updateQuery = $this->db->prepare("UPDATE users SET password = :password WHERE id = :id");
        $updateQuery->execute(['password' => $hashedPassword, 'id' => $userId]);

        echo "Mot de passe modifié avec succès.";
    }
}


    
    
}
?>


