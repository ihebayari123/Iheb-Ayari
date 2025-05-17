
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>
<body>
<a href="resolvereclamation.php?id=93">spprimer la réclamation</a>

</body>
</html>
<?php
include '../../Controller/reclamationController.php';

// Check if 'id' is set in the URL and is a valid number
if (isset($_GET['id']) && is_numeric($_GET['id'])) {
    $id = $_GET['id'];

    // Create an instance of the reclamationController
    $gestionreclamationC = new reclamationController();

    // Delete the reclamation with the specified id
    $gestionreclamationC->deletereclamation($id);

    // Redirect to the reclamation list page after successful deletion
    header('Location: reclamationlist.php');
    exit; // Always use exit after header redirect
} else {
    // If 'id' is not set or is invalid, display an error message
    echo "Invalid reclamation ID.";
    exit;
}
?>
