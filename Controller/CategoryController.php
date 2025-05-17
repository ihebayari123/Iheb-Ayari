<?php
include_once __DIR__ . '/../config.php';
include_once __DIR__ . '/../Model/CategoryModel.php';

class CategoryController {
    // Retrieve all categories
    public function listCategories() {
        $sql = "SELECT * FROM categories";
        $db = config::getConnexion();

        try {
            $stmt = $db->query($sql);
            return $stmt->fetchAll(PDO::FETCH_ASSOC);
        } catch (PDOException $e) {
            return ['error' => $e->getMessage()];
        }
    }

    // Add a new category
    public function addCategory(Category $category) {
        $sql = "INSERT INTO categories (name, description) VALUES (:name, :description)";
        $db = config::getConnexion();

        try {
            $stmt = $db->prepare($sql);
            $stmt->execute([
                ':name' => $category->getName(),
                ':description' => $category->getDescription()
            ]);
            return ['success' => 'Category added successfully.'];
        } catch (PDOException $e) {
            return ['error' => $e->getMessage()];
        }
    }

    // Delete a category
    public function deleteCategory(int $id) {
        $sql = "DELETE FROM categories WHERE id = :id";
        $db = config::getConnexion();

        try {
            $stmt = $db->prepare($sql);
            $stmt->bindValue(':id', $id, PDO::PARAM_INT);
            $stmt->execute();
            return ['success' => 'Category deleted successfully.'];
        } catch (PDOException $e) {
            return ['error' => $e->getMessage()];
        }
    }

    // Update a category
    public function updateCategory(Category $category) {
        $sql = "UPDATE categories SET name = :name, description = :description WHERE id = :id";
        $db = config::getConnexion();

        try {
            $stmt = $db->prepare($sql);
            $stmt->execute([
                ':id' => $category->getId(),
                ':name' => $category->getName(),
                ':description' => $category->getDescription()
            ]);
            return ['success' => 'Category updated successfully.'];
        } catch (PDOException $e) {
            return ['error' => $e->getMessage()];
        }
    }

    // Retrieve a single category by ID
    public function getCategory(int $id): ?Category {
        $sql = "SELECT * FROM categories WHERE id = :id";
        $db = config::getConnexion();

        try {
            $stmt = $db->prepare($sql);
            $stmt->bindParam(':id', $id, PDO::PARAM_INT);
            $stmt->execute();
            $data = $stmt->fetch(PDO::FETCH_ASSOC);

            if ($data) {
                return new Category($data['id'], $data['name'], $data['description']);
            }
            return null;
        } catch (PDOException $e) {
            return null;
        }
    }
}
class CategoryC {
    // Fetch all products
    public function afficheAllProduits() {
        try {
            $pdo = config::getconnexion();
            $query = $pdo->query("SELECT * FROM produits");
            return $query->fetchAll();
        } catch (PDOException $e) {
            echo $e->getMessage();
        }
    }

    // Existing methods
    public function afficheproduits($name) {
        try {
            $pdo = config::getconnexion();
            $query = $pdo->prepare("SELECT * FROM produits WHERE category = :id");
            $query->execute(['id' => $name]);
            return $query->fetchAll();
        } catch (PDOException $e) {
            echo $e->getMessage();
        }
    }

    public function affichercategories() {
        try {
            $pdo = config::getconnexion();
            $query = $pdo->query("SELECT * FROM categories");
            return $query->fetchAll();
        } catch (PDOException $e) {
            echo $e->getMessage();
        }
    }
}


?>
 