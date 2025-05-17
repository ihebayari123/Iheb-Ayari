<?php
require_once '../../config.php';

class Database {
    private static $connection = null;

    public static function getConnection() {
        if (self::$connection === null) {
            global $conn;
            self::$connection = $conn;
        }
        return self::$connection;
    }
}

class Order {
    private $id;
    private $customerName;
    private $customerEmail; // Nouvelle propriété pour l'email
    private $productName;
    private $quantity;
    private $price;
    private $status;

    public function __construct($id = null, $customerName, $customerEmail, $productName, $quantity, $price, $status = 'pending') {
        $this->id = $id;
        $this->customerName = $customerName;
        $this->customerEmail = $customerEmail; // Affectation de l'email
        $this->productName = $productName;
        $this->quantity = $quantity;
        $this->price = $price;
        $this->status = $status;
    }

    public function save() {
        $db = Database::getConnection();

        if ($this->id) {
            // Requête de mise à jour incluant l'email
            $stmt = $db->prepare("UPDATE orders SET customer_name = ?, customer_email = ?, product_name = ?, quantity = ?, price = ?, status = ? WHERE id = ?");
            $stmt->execute([$this->customerName, $this->customerEmail, $this->productName, $this->quantity, $this->price, $this->status, $this->id]);
        } else {
            // Requête d'insertion incluant l'email
            $stmt = $db->prepare("INSERT INTO orders (customer_name, customer_email, product_name, quantity, price, status) VALUES (?, ?, ?, ?, ?, ?)");
            $stmt->execute([$this->customerName, $this->customerEmail, $this->productName, $this->quantity, $this->price, $this->status]);
            $this->id = $db->lastInsertId();
        }
    }

    public static function all() {
        $db = Database::getConnection();
        $stmt = $db->query("SELECT * FROM orders");
        return $stmt->fetchAll(PDO::FETCH_ASSOC);
    }

    public function getId() {
        return $this->id;
    }

    public function getCustomerName() {
        return $this->customerName;
    }

    public function getCustomerEmail() { // Getter pour l'email
        return $this->customerEmail;
    }

    public function getProductName() {
        return $this->productName;
    }

    public function getQuantity() {
        return $this->quantity;
    }

    public function getPrice() {
        return $this->price;
    }

    public function getStatus() {
        return $this->status;
    }
}
class Delivery {

    // Récupérer l'état de la livraison d'une commande
    public function getDeliveryStatus($orderId) {
        $db = Database::getConnection();  // Utilisation de la méthode correcte
        $stmt = $db->prepare("SELECT * FROM delivery WHERE order_id = ?");
        $stmt->execute([$orderId]);
        
        return $stmt->fetch(PDO::FETCH_ASSOC);  // Retourner le résultat sous forme de tableau associatif
    }
    
    // Mettre à jour le statut de la livraison
    public function updateStatus($orderId, $status) {
        $db = Database::getConnection();  // Utilisation de la méthode correcte
        $stmt = $db->prepare("UPDATE delivery SET status = ? WHERE order_id = ?");
        $stmt->execute([$status, $orderId]);
    }
}

?>
