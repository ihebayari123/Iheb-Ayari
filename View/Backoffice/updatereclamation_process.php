<?php
include '../../Controller/reclamationController.php';


if (isset($_POST['id'], $_POST['nom'], $_POST['produit'], $_POST['description'], $_POST['date'], $_POST['statut'])) {
    // Récupérer les données du formulaire
    $id = $_POST['id'];
    $nom = $_POST['nom'];
    $produit = $_POST['produit'];
    $description = $_POST['description'];
    $date = $_POST['date'];
    $statut = $_POST['statut'];

    // Créer une instance du contrôleur
    $reclamationController = new reclamationController();

    // Appeler la méthode pour mettre à jour la réclamation
    $updated = $reclamationController->updatereclamation($id, $nom, $produit, $description, $date, $statut);

    if ($updated) {
        // Rediriger vers la liste des réclamations après la mise à jour
        header('Location: reclamationlist.php');
        exit();
    } else {
        echo "Une erreur est survenue lors de la mise à jour.";
    }
} else {
    // Afficher un message d'erreur détaillé
    echo "Erreur : les données suivantes sont manquantes :<br>";
    if (!isset($_POST['id'])) echo "ID manquant<br>";
    if (!isset($_POST['nom'])) echo "Nom manquant<br>";
    if (!isset($_POST['produit'])) echo "Produit manquant<br>";
    if (!isset($_POST['description'])) echo "Description manquante<br>";
    if (!isset($_POST['date'])) echo "Date manquante<br>";
    if (!isset($_POST['statut'])) echo "Statut manquant<br>";
}
?>
