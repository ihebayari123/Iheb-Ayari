<?php
// Include the CategoryController
include_once "../../controller/CategoryController.php";

// Instantiate the controller
$CategoryController = new CategoryC();

// Traitement du formulaire
$list = [];
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    if (isset($_POST['Category']) && isset($_POST['search'])) {
        $name = htmlspecialchars($_POST['Category']); // Sanitize input
        $list = $CategoryController->afficheproduits($name); // Fetch products by category
    }
}

// Retrieve all categories for the dropdown
$categories = $CategoryController->affichercategories();
?>

<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Recherche de Produits par Catégorie</title>
    <link href="https://cdn.jsdelivr.net/npm/tailwindcss@2.2.19/dist/tailwind.min.css" rel="stylesheet">
</head>
<body class="bg-gray-100 p-8">
    <div class="max-w-3xl mx-auto bg-white shadow-lg rounded-lg p-6">
        <h1 class="text-2xl font-bold text-center mb-4">Recherche de Produits par Catégorie</h1>
        <form action="" method="POST" class="mb-6">
            <label for="Category" class="block text-lg font-semibold mb-2">Sélectionnez une catégorie :</label>
            <select name="Category" id="Category" class="w-full border rounded-lg p-2">
                <?php
                foreach ($categories as $category) {
                    echo '<option value="' . htmlspecialchars($category['name']) . '">' . htmlspecialchars($category['name']) . '</option>';
                }
                ?>
            </select>
            <button type="submit" name="search" class="mt-4 px-4 py-2 bg-blue-500 text-white font-semibold rounded-lg hover:bg-blue-600">
                Rechercher
            </button>
        </form>

        <?php if (!empty($list)) { ?>
            <h2 class="text-xl font-semibold mb-4">Produits correspondants à la catégorie sélectionnée :</h2>
            <ul class="list-disc pl-6">
                <?php foreach ($list as $produit) { ?>
                    <li class="mb-2">
                        <?= htmlspecialchars($produit['name']) ?> - <?= htmlspecialchars($produit['price']) ?> dt
                    </li>
                <?php } ?>
            </ul>
        <?php } else if ($_SERVER["REQUEST_METHOD"] == "POST") { ?>
            <p class="text-red-500 font-semibold">Aucun produit trouvé pour cette catégorie.</p>
        <?php } ?>
    </div>
</body>
</html>
