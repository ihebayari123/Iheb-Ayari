<?php
include '../../Controller/userController.php';

if (isset($_GET['id'])) {
    $id = $_GET['id'];

    $userC = new userController();
    $deleted = $userC->deleteuser($id); // Appel de la méthode de suppression

    if ($deleted) {
        // Rediriger vers la liste des utilisateurs après suppression
        header("Location: userList.php");
        exit();
    } else {
        echo "Error deleting user.";
    }
}
?>
