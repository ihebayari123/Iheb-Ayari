<?php
require_once '../../model/model.php';  // Inclure le modèle
require_once '../../config.php';  // Inclure la connexion à la base de données

class OrderController {
    private $db;

    public function __construct($db) {
        $this->db = $db;
    }

    // ==================== ORDERS ====================

    public function addOrder($customerName, $customerEmail, $productName, $quantity, $price) {
        $stmt = $this->db->prepare("INSERT INTO orders (customer_name, customer_email, product_name, quantity, price, status) VALUES (?, ?, ?, ?, ?, 'Pending')");
        $stmt->execute([$customerName, $customerEmail, $productName, $quantity, $price]);
        return $stmt->rowCount() > 0;
    }

    public function showOrders() {
        $orders = Order::all();  // Utilise la méthode `all()` du modèle Order
        return $orders;
    }

    public function getOrderById($id) {
        $stmt = $this->db->prepare("SELECT * FROM orders WHERE id = ?");
        $stmt->execute([$id]);
        return $stmt->fetch(PDO::FETCH_ASSOC);
    }

    public function editOrder($id, $customerName, $customerEmail, $productName, $quantity, $price) {
        $stmt = $this->db->prepare("UPDATE orders SET customer_name = ?, customer_email = ?, product_name = ?, quantity = ?, price = ? WHERE id = ?");
        $stmt->execute([$customerName, $customerEmail, $productName, $quantity, $price, $id]);
        return $stmt->rowCount() > 0;
    }

    public function deleteOrder($id) {
        $stmt = $this->db->prepare("DELETE FROM orders WHERE id = ?");
        $stmt->execute([$id]);
        return $stmt->rowCount() > 0;
    }

    // ==================== DELIVERIES ====================

    // Ajouter une livraison
    public function addDelivery($orderId, $deliveryStatus, $deliveryDate) {
        $stmt = $this->db->prepare("INSERT INTO deliveries (order_id, delivery_status, delivery_date) VALUES (?, ?, ?)");
        $stmt->execute([$orderId, $deliveryStatus, $deliveryDate]);
        return $stmt->rowCount() > 0;
    }
    

    // Afficher toutes les livraisons
    public function showDeliveries() {
        $stmt = $this->db->prepare("SELECT * FROM deliveries");
        $stmt->execute();
        return $stmt->fetchAll(PDO::FETCH_ASSOC);
    }

    // Récupérer une livraison par ID
    public function getDeliveryById($id) {
        $stmt = $this->db->prepare("SELECT * FROM deliveries WHERE id = ?");
        $stmt->execute([$id]);
        return $stmt->fetch(PDO::FETCH_ASSOC);
    }

    // Modifier une livraison
    public function editDelivery($id, $status, $deliveryDate) {
        $stmt = $this->db->prepare("UPDATE deliveries SET delivery_status = ?, delivery_date = ? WHERE id = ?");
        $stmt->execute([$status, $deliveryDate, $id]);
        return $stmt->rowCount() > 0;
    }

    // Supprimer une livraison
    public function deleteDelivery($id) {
        $stmt = $this->db->prepare("DELETE FROM deliveries WHERE id = ?");
        $stmt->execute([$id]);
        return $stmt->rowCount() > 0;
    }

    // ==================== Récupérer les commandes pour la sélection ====================

    // Récupérer toutes les commandes pour afficher l'order_id dans le formulaire de livraison
    public function getAllOrders() {
        $stmt = $this->db->prepare("SELECT id, customer_name FROM orders");
        $stmt->execute();
        return $stmt->fetchAll(PDO::FETCH_ASSOC);
    }

    // ==================== Vérification des statuts des commandes ====================

    // Vérifie si une commande est éligible à une livraison
    public function isValidOrderForDelivery($orderId) {
        $stmt = $this->db->prepare("SELECT status FROM orders WHERE id = ?");
        $stmt->execute([$orderId]);
        $order = $stmt->fetch(PDO::FETCH_ASSOC);

        if ($order) {
            return $order['status'] !== 'Livré' && $order['status'] !== 'Annulé';
        }
        return false;
    }
    public function showDeliveriesFiltered($searchTerm, $sortBy, $sortOrder) {
        $validSortColumns = ['id', 'order_id', 'delivery_status', 'delivery_date'];
        $sortBy = in_array($sortBy, $validSortColumns) ? $sortBy : 'id';
        $sortOrder = strtoupper($sortOrder) === 'DESC' ? 'DESC' : 'ASC';
    
        $query = "SELECT * FROM deliveries WHERE 
                  delivery_status LIKE :search 
                  OR delivery_date LIKE :search 
                  OR order_id LIKE :search 
                  ORDER BY $sortBy $sortOrder";
    
        $stmt = $this->db->prepare($query);
        $stmt->execute([':search' => "%$searchTerm%"]);
        return $stmt->fetchAll(PDO::FETCH_ASSOC);
    }
    
}
?>
