<?php
require_once '../../controller/controller.php';  // Assurez-vous que le contrôleur est bien inclus
require_once '../../config.php';  // Inclure la connexion à la base de données
$conn = config::getConnexion();
$orderController = new OrderController($conn);

if (isset($_GET['id'])) {
    $id = $_GET['id'];

    // Appeler la méthode deleteOrder pour supprimer la commande
    if ($orderController->deleteOrder($id)) {
        echo "Commande supprimée avec succès!";
    } else {
        echo "Erreur lors de la suppression de la commande.";
    }
} else {
    echo "ID de commande non spécifié.";
}
?>
