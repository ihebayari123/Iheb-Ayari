<?php
include '../../config.php';

if (isset($_GET['id'])) {
    $order_id = $_GET['id'];

    // Requête préparée avec PDO
    $sql = "SELECT * FROM orders WHERE id = :id";
    $stmt = $conn->prepare($sql);
    $stmt->bindParam(':id', $order_id, PDO::PARAM_INT);
    $stmt->execute();
    $order = $stmt->fetch(PDO::FETCH_ASSOC);

    if (!$order) {
        die("Commande non trouvée !");
    }

    if ($_SERVER['REQUEST_METHOD'] == 'POST') {
        // Récupération des valeurs du formulaire
        $customer_name = $_POST['customer_name'];
        $customer_email = $_POST['customer_email'];
        $product_name = $_POST['product_name'];
        $quantity = $_POST['quantity'];
        $price = $_POST['price'];

        // Requête pour mettre à jour la commande
        $update_sql = "UPDATE orders SET customer_name = :customer_name, customer_email = :customer_email, product_name = :product_name, quantity = :quantity, price = :price WHERE id = :id";
        $update_stmt = $conn->prepare($update_sql);

        // Lier les paramètres avec bindParam() pour PDO
        $update_stmt->bindParam(':customer_name', $customer_name, PDO::PARAM_STR);
        $update_stmt->bindParam(':customer_email', $customer_email, PDO::PARAM_STR);
        $update_stmt->bindParam(':product_name', $product_name, PDO::PARAM_STR);
        $update_stmt->bindParam(':quantity', $quantity, PDO::PARAM_INT);
        $update_stmt->bindParam(':price', $price, PDO::PARAM_STR);  // Utilisation de PDO::PARAM_STR pour un prix en format décimal
        $update_stmt->bindParam(':id', $order_id, PDO::PARAM_INT);

        if ($update_stmt->execute()) {
            header("Location: ordersummary.php?status=success_edit");
            exit();
        } else {
            echo "Erreur lors de la mise à jour : " . $conn->errorInfo()[2];  // Affiche l'erreur PDO si la mise à jour échoue
        }
    }
} else {
    die("ID de commande non fourni !");
}
?>

<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="assets/css/main.css">
    <title>Modifier la commande</title>
</head>

<body>

    <h1>Modifier la commande</h1>
    <form method="POST" action="" onsubmit="return validateOrderForm()">
        <label for="customer_name">Nom du client :</label>
        <input type="text" name="customer_name" id="customer_name" value="<?php echo htmlspecialchars($order['customer_name']); ?>"><br>

        <label for="customer_email">Email :</label>
        <input type="email" name="customer_email" id="customer_email" value="<?php echo htmlspecialchars($order['customer_email']); ?>"><br>

        <label for="product_name">Produit :</label>
        <input type="text" name="product_name" id="product_name" value="<?php echo htmlspecialchars($order['product_name']); ?>"><br>

        <label for="quantity">Quantité :</label>
        <input type="number" name="quantity" id="quantity" value="<?php echo htmlspecialchars($order['quantity']); ?>"><br>

        <label for="price">Prix :</label>
        <input type="number" step="0.01" name="price" id="price" value="<?php echo htmlspecialchars($order['price']); ?>"><br>

        <button type="submit">Mettre à jour</button>
    </form>
    <script src="form_validation.js"></script>
    
</body>
</html>
