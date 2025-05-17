<?php
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "agriaura";

try {
    // Connexion à la base de données avec PDO
    $conn = new PDO("mysql:host=$servername;dbname=$dbname", $username, $password);
    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
} catch (PDOException $e) {
    die("Connexion échouée : " . $e->getMessage());
}

if (!isset($_GET['id'])) {
    die("ID de commande non spécifié.");
}

$order_id = intval($_GET['id']);

// Requête DELETE avec PDO
$sql = "DELETE FROM orders WHERE id = :id";
$stmt = $conn->prepare($sql);

// Liaison du paramètre
$stmt->bindParam(':id', $order_id, PDO::PARAM_INT);

if ($stmt->execute()) {
    header("Location: ordersummary.php?status=success_delete");
    exit();
} else {
    echo "Erreur lors de la suppression : " . implode(", ", $stmt->errorInfo());
}

$conn = null;  // Fermeture de la connexion PDO
?>
