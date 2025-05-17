<?php
require_once '../../controller/controller.php'; // Inclure le contrôleur
require_once '../../config.php'; // Inclure la connexion à la base de données
$conn = config::getConnexion();
$orderController = new OrderController($conn);

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $deliveryId = $_POST['id'];

    if ($orderController->deleteDelivery($deliveryId)) {
        echo "Livraison supprimée avec succès!";
        header("Location: deliverytables.php");  // Redirige vers la liste des commandes ou une autre page
        exit;
    } else {
        echo "Erreur lors de la suppression de la livraison.";
    }
}
?>
