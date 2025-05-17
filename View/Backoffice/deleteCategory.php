<?php
include '../../Controller/CategoryController.php';

if (isset($_GET['id']) && is_numeric($_GET['id'])) {
    $categoryId = intval($_GET['id']);
    $controller = new CategoryController();

    $result = $controller->deleteCategory($categoryId);

    if (isset($result['success'])) {
        header('Location: listCategories.php?message=Catégorie supprimée avec succès.');
    } else {
        header('Location: listCategories.php?error=' . urlencode($result['error']));
    }
} else {
    header('Location: listCategories.php?error=ID de catégorie invalide.');
}
exit();
