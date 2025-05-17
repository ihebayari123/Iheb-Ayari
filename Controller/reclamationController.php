<?php
include(__DIR__ . '/../config.php');
include(__DIR__ . '/../Model/gestion.php');
require __DIR__ . '/../vendor/autoload.php';
    
use PHPMailer\PHPMailer\PHPMailer;
use PHPMailer\PHPMailer\Exception;
class reclamationController {

    // List all reclamations
    public function listreclamation($sortColumn = 'id', $sortOrder = 'ASC') {
        // Validation des paramètres pour éviter les injections SQL
        $allowedColumns = ['id', 'nom'];
        $allowedOrders = ['ASC', 'DESC'];
    
        $sortColumn = in_array($sortColumn, $allowedColumns) ? $sortColumn : 'id';
        $sortOrder = in_array($sortOrder, $allowedOrders) ? $sortOrder : 'ASC';
    
        $sql = "SELECT * FROM gestion ORDER BY $sortColumn $sortOrder";
        $db = config::getConnexion();
        try {
            return $db->query($sql)->fetchAll();
        } catch (Exception $e) {
            die('Erreur: ' . $e->getMessage());
        }
    }
    public function searchReclamationByName($name)
    {
        $db = config::getConnexion();
        try {
            $query = $db->prepare('SELECT * FROM gestion WHERE nom LIKE :name');
            $query->execute(['name' => '%' . $name . '%']);
            return $query->fetchAll();
        } catch (PDOException $e) {
            echo 'Erreur : ' . $e->getMessage();
        }
    }
    
    public function sendReclamationEmail($nom, $produit, $description, $date, $statut) {
        $mail = new PHPMailer(true);
    
        try {
            // Configuration du serveur SMTP
            $mail->isSMTP();
            $mail->Host = 'smtp.gmail.com';
            $mail->SMTPAuth = true;
            $mail->Username = 'haningmati3@gmail.com'; // Remplacez par votre email
            $mail->Password = 'wtsr jfqx rfrz qyzq'; // Remplacez par le mot de passe d'application
            $mail->SMTPSecure = PHPMailer::ENCRYPTION_STARTTLS;
            $mail->Port = 587;
    
            // Destinataire
            $mail->setFrom('no-reply@example.com', 'Système de Réclamations');
            $mail->addAddress('haningmati3@gmail.com', 'Administrateur'); // Email de destination
    
            $mail->isHTML(true);
            $mail->Subject = 'Nouvelle réclamation soumise';
            $mail->Body = "
                <p>Une nouvelle réclamation a été soumise :</p>
                <ul>
                    <li><b>Nom :</b> {$_POST['nom']}</li>
                    <li><b>Produit :</b> {$_POST['produit']}</li>
                    <li><b>Description :</b> {$_POST['description']}</li>
                    <li><b>Date :</b> {$_POST['date']}</li>
                    <li><b>Email :</b> {$_POST['email']}</li>
                    <li><b>CIN :</b> {$_POST['cin']}</li>
                </ul>
            ";
    
            // Envoi de l'email
           
            echo "Email envoyé avec succès.";
        } catch (Exception $e) {
            echo "Erreur lors de l'envoi de l'email : {$mail->ErrorInfo}";
        }
    }
    
    

    
    public function listResolutionReclamations() {
        $sql = "SELECT * FROM resoudre_reclamation"; 
        $db = config::getConnexion();
        try {
            $query = $db->prepare($sql);
            $query->execute();
            return $query->fetchAll();
        } catch (Exception $e) {
            die('Erreur: ' . $e->getMessage());
        }
    }
    
    // Add a new reclamation
  
    public function addreclamation($reclamation) {
        $db = config::getConnexion(); 
    
        $sql = "INSERT INTO gestion (nom, produit, description, date, statut) 
                VALUES (:nom, :produit, :description, :date, :statut)";
    
        try {
            $query = $db->prepare($sql);
    
            // Exécuter la requête avec les valeurs dynamiques
            $query->execute([
                ':nom' => $reclamation->getnom(),
                ':produit' => $reclamation->getproduit(),
                ':description' => $reclamation->getdescription(),
                ':date' => $reclamation->getdate()->format('Y-m-d'), // Format correct pour une date SQL
                ':statut' => $reclamation->getstatut()
            ]);
    
            // Après l'ajout de la réclamation, envoyer un email à l'administrateur
            $this->sendReclamationEmail(
                $reclamation->getnom(),
                $reclamation->getproduit(),
                $reclamation->getdescription(),
                $reclamation->getdate()->format('Y-m-d'),
                $reclamation->getstatut()
            );
    
            echo "Réclamation ajoutée et email envoyé à l'administrateur.";
        } catch (Exception $e) {
            die('Erreur lors de l\'ajout de la réclamation : ' . $e->getMessage());
        }
    }
    
    //delete resolution
    public function deleteResolution($id) {
        $sql = "DELETE FROM resoudre_reclamation WHERE id = :id";
        $db = config::getConnexion();
        
        try {
            $query = $db->prepare($sql);
            $query->bindValue(':id', $id);
            $query->execute();
        } catch (Exception $e) {
            die('Erreur: ' . $e->getMessage());
        }
    }
    
    public function getResolutionById($id) {
    $sql = "SELECT * FROM resoudre_reclamation WHERE id = :id";
    $db = config::getConnexion();
    
    try {
        $query = $db->prepare($sql);
        $query->bindValue(':id', $id);
        $query->execute();
        return $query->fetch();
    } catch (Exception $e) {
        die('Erreur: ' . $e->getMessage());
    }
}

public function sortReclamations($column, $order) {
    $db = config::getConnexion();
    $validColumns = ['id', 'nom']; // Allowed columns for sorting
    $validOrders = ['ASC', 'DESC']; // Allowed sorting orders

    // Validate inputs to avoid SQL injection
    if (!in_array($column, $validColumns) || !in_array($order, $validOrders)) {
        throw new Exception("Invalid sorting parameters");
    }

    $sql = "SELECT * FROM gestion ORDER BY $column $order";

    try {
        $query = $db->prepare($sql);
        $query->execute();
        return $query->fetchAll(PDO::FETCH_ASSOC);
    } catch (Exception $e) {
        error_log("Error during sorting: " . $e->getMessage());
        return [];
    }
}





public function updateResolution($id, $statut, $assigned_to, $description, $method, $follow_up_date) {
    $sql = "UPDATE resoudre_reclamation 
            SET statut = :statut, assigned_to = :assigned_to, resolution_description = :description, resolution_method = :method, follow_up_date = :follow_up_date
            WHERE id = :id";
    $db = config::getConnexion();
    
    try {
        $query = $db->prepare($sql);
        $query->bindValue(':id', $id);
        $query->bindValue(':statut', $statut);
        $query->bindValue(':assigned_to', $assigned_to);
        $query->bindValue(':description', $description);
        $query->bindValue(':method', $method);
        $query->bindValue(':follow_up_date', $follow_up_date);
        $query->execute();
    } catch (Exception $e) {
        die('Erreur: ' . $e->getMessage());
    }
}


    // Delete a reclamation by ID
    public function deletereclamation($id) {
        $sql = "DELETE FROM gestion WHERE id = :id";
        $db = config::getConnexion();

        try {
            // Prepare the delete query
            $query = $db->prepare($sql);
            $query->bindValue(':id', $id, PDO::PARAM_INT); // Bind ID as an integer
            $query->execute();

            echo "Reclamation deleted successfully.";
        } catch(Exception $e) {
            error_log("Error occurred: " . $e->getMessage()); // Log error to server
            echo "An error occurred. Please try again later."; // Generic error message
        }
    }
   


    // Get a single reclamation by ID
    public function getreclamation($id) {
        $sql = "SELECT * FROM gestion WHERE id = :id";
        $db = config::getConnexion();

        try {
            // Prepare and execute the select query
            $query = $db->prepare($sql);
            $query->execute(['id' => $id]);

            // Fetch the reclamation as an associative array
            $reclamation = $query->fetch(PDO::FETCH_ASSOC);
            return $reclamation;
        } catch(Exception $e) {
            error_log("Error occurred: " . $e->getMessage()); // Log error to server
            echo "An error occurred. Please try again later."; // Generic error message
        }
    }
    public function resolverReclamationAndSave($id, $statut, $assigned_to, $resolution_description, $resolution_method, $follow_up_date) {
        $db = config::getConnexion();
    
        // Insérer dans la table 'resoudre_reclamation'
        $sql = "INSERT INTO resoudre_reclamation (reclamation_id, statut, resolution_date, assigned_to, resolution_description, resolution_method, follow_up_date) 
                VALUES (:reclamation_id, :statut, NOW(), :assigned_to, :resolution_description, :resolution_method, :follow_up_date)";
        
        try {
            $query = $db->prepare($sql);
            $query->execute([
                ':reclamation_id' => $id,
                ':statut' => $statut,
                ':assigned_to' => $assigned_to,
                ':resolution_description' => $resolution_description,
                ':resolution_method' => $resolution_method,
                ':follow_up_date' => $follow_up_date
            ]);
    
            // Mettre à jour le statut de la réclamation dans la table 'gestion'
            $update_sql = "UPDATE gestion SET statut = :statut WHERE id = :id";
            $update_query = $db->prepare($update_sql);
            $update_query->execute([
                ':statut' => $statut,
                ':id' => $id
            ]);
    
            return true;
        } catch (Exception $e) {
            error_log("Error occurred: " . $e->getMessage());
            return false;
        }
    }
    
 //modifier une reclamation  
 
 public function updatereclamation($id, $nom, $produit, $description, $date, $statut) {
    // Connexion à la base de données
    $db = config::getConnexion();

    // Requête SQL pour mettre à jour les informations de la réclamation
    $sql = "UPDATE gestion 
            SET nom = :nom, 
                produit = :produit, 
                description = :description, 
                date = :date, 
                statut = :statut 
            WHERE id = :id";

    try {
        // Préparer la requête
        $query = $db->prepare($sql);

        // Lier les valeurs à la requête préparée
        $query->bindParam(':id', $id);
        $query->bindParam(':nom', $nom);
        $query->bindParam(':produit', $produit);
        $query->bindParam(':description', $description);
        $query->bindParam(':date', $date);
        $query->bindParam(':statut', $statut);

        // Exécuter la requête
        $query->execute();

        // Vérifier si la mise à jour a bien eu lieu
        if ($query->rowCount() > 0) {
            return true; // La mise à jour a été effectuée avec succès
        } else {
            return false; // Aucun 
        }
    } catch (Exception $e) {
        // En cas d'erreur, enregistrer l'erreur et retourner false
        error_log("Erreur lors de la mise à jour de la réclamation : " . $e->getMessage());
        return false;
    }
}

public function searchReclamations($field, $value) {
    $db = config::getConnexion();

    // Construire la requête dynamique en fonction du champ recherché
    $sql = "SELECT * FROM gestion WHERE $field LIKE :value";

    try {
        $query = $db->prepare($sql);
        $query->execute([':value' => '%' . $value . '%']);
        return $query->fetchAll(PDO::FETCH_ASSOC); // Retourner les résultats sous forme de tableau associatif
    } catch (Exception $e) {
        error_log("Erreur lors de la recherche : " . $e->getMessage());
        return []; // Retourne un tableau vide en cas d'erreur
    }
}


}
?>
