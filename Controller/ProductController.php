<?php
include(__DIR__ . '/../config.php');
include(__DIR__ . '/../Model/ProductModel.php');

class ProductController {
    private $db; // Declare $db as a private property

    // Initialize $db in the constructor
    public function __construct() {
        try {
            $this->db = config::getConnexion(); // Use the database connection method
        } catch (PDOException $e) {
            die("Database connection failed: " . $e->getMessage());
        }
    }

    // List all products
    public function listProducts() {
        $sql = "SELECT * FROM produits";

        try {
            $stmt = $this->db->query($sql); // Use $this->db
            return $stmt->fetchAll(PDO::FETCH_ASSOC);
        } catch (PDOException $err) {
            return ['error' => $err->getMessage()];
        }
    }

    // Get a single product by ID
    public function getProduct(int $id): ?Product {
        $query = "SELECT * FROM produits WHERE id = :id";

        try {
            $statement = $this->db->prepare($query); // Use $this->db
            $statement->bindParam(':id', $id, PDO::PARAM_INT);
            $statement->execute();
            $data = $statement->fetch(PDO::FETCH_ASSOC);

            if ($data) {
                return new Product(
                    $data['id'],
                    $data['name'],
                    $data['description'],
                    $data['price'],
                    $data['availability'],
                    $data['category']
                );
            }
            return null; // Return null if no product is found
        } catch (PDOException $err) {
            return null; // Handle exception and return null
        }
    }
   // Check for unique product in a category
private function isProductUniqueInCategory(string $name, string $category): bool {
    $sql = "SELECT COUNT(*) FROM produits WHERE name = :name AND category = :category";
    $query = $this->db->prepare($sql);
    $query->execute([':name' => $name, ':category' => $category]);
    return $query->fetchColumn() == 0;
}

    // Add a new product
    public function addProduct(Product $product) {
        $errors = $this->validateProduct($product);

        if (!empty($errors)) {
            return ['errors' => $errors];
        }
         // Check for duplicate product
        if (!$this->isProductUniqueInCategory($product->getName(), $product->getCategory())) {
        return ['error' => 'Product already exists in this category.'];
        }


        $sql = "INSERT INTO produits (id, name, description, price, availability, category) 
                VALUES (:id, :name, :description, :price, :availability, :category)";

        try {
            $query = $this->db->prepare($sql); // Use $this->db
            $query->execute([
                ':id' => $product->getId(),
                ':name' => $product->getName(),
                ':description' => $product->getDescription(),
                ':price' => $product->getPrice(),
                ':availability' => $product->getAvailability(),
                ':category' => $product->getCategory()
            ]);
            return ['success' => 'Product added successfully.',
            'productId' => $this->db->lastInsertId()
        ];
        } catch (PDOException $err) {
            if ($err->getCode() === '23000') { // Integrity constraint violation
                return ['error' => 'Product violates a database constraint.'];
            }
            error_log($err->getMessage(), 3, '/path/to/error.log');
            return ['error' => 'Database error: ' . $err->getMessage()];
        }
    }

    // Delete a product by ID
    public function deleteProduct(int $id) {
        $sql = "DELETE FROM produits WHERE id = :id";

        try {
            $query = $this->db->prepare($sql); // Use $this->db
            $query->bindValue(':id', $id, PDO::PARAM_INT);
            $query->execute();
            return ['success' => 'Product deleted successfully.'];
        } catch (PDOException $err) {
            return ['error' => $err->getMessage()];
        }
    }

    // Update a product by ID
    public function updateProduct(Product $product) {
        $errors = $this->validateProduct($product);

        if (!empty($errors)) {
            return ['errors' => $errors];
        }

        $sql = "UPDATE produits 
                SET name = :name, description = :description, price = :price, 
                    availability = :availability, category = :category 
                WHERE id = :id";

        try {
            $query = $this->db->prepare($sql); // Use $this->db
            $query->execute([
                ':id' => $product->getId(),
                ':name' => $product->getName(),
                ':description' => $product->getDescription(),
                ':price' => $product->getPrice(),
                ':availability' => $product->getAvailability(),
                ':category' => $product->getCategory()
            ]);
            return ['success' => 'Product updated successfully.'];
        } catch (PDOException $err) {
            return ['error' => $err->getMessage()];
        }
    }

    // Validate product data
    private function validateProduct(Product $product) {
        $errors = [];
        $name = $product->getName();
        $description = $product->getDescription();
        $price = $product->getPrice();
        $category = $product->getCategory();

        if (empty($name)) {
            $errors[] = "Name is required.";
        }

        if (empty($description)) {
            $errors[] = "Description is required.";
        }

        if (empty($price) || !is_numeric($price) || $price <= 0) {
            $errors[] = "Price must be a positive number.";
        }

        if (empty($category)) {
            $errors[] = "Category is required.";
        }

        return $errors;
    }
}
?>
