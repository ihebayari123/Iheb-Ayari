<?php
include '../../Controller/reclamationController.php';

if (isset($_GET['id'])) {
    $id = $_GET['id'];
    $gestionreclamationC = new reclamationController();
    
    // Appeler la méthode du contrôleur pour supprimer la résolution
    $gestionreclamationC->deleteResolution($id);

    // Rediriger après la suppression
    header('Location: reclamationlist.php'); // Redirige vers la liste des réclamations
}
?>
