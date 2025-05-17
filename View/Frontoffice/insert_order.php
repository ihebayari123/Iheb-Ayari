<?php
// Inclure la classe de configuration
include('../../config.php');

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    
    // Récupération des données du formulaire
    $customer_name = $_POST['customer_name'];
    $customer_email = $_POST['customer_email'];
    $product_name = $_POST['product_name'];
    $quantity = $_POST['quantity'];
    $price = $_POST['price'];

    // Préparation de la requête d'insertion avec des paramètres
    $sql = "INSERT INTO orders (customer_name, customer_email, product_name, quantity, price) 
            VALUES (:customer_name, :customer_email, :product_name, :quantity, :price)";

    try {
        // Récupérer la connexion via la méthode statique
        $conn = config::getConnexion();

        // Préparation de la requête
        $stmt = $conn->prepare($sql);
        
        // Lier les paramètres
        $stmt->bindParam(':customer_name', $customer_name);
        $stmt->bindParam(':customer_email', $customer_email);
        $stmt->bindParam(':product_name', $product_name);
        $stmt->bindParam(':quantity', $quantity, PDO::PARAM_INT);
        $stmt->bindParam(':price', $price, PDO::PARAM_STR);

        // Exécution de la requête
        $stmt->execute();

        header("Location: ordersummary.php");
        exit();
    } catch (PDOException $e) {
        // Gestion des erreurs d'exécution de la requête
        echo "Erreur: " . $e->getMessage();
    }
}
?>
