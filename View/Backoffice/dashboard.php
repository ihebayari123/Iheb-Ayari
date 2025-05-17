<?php
// Include the CategoryController
include_once "../../controller/CategoryController.php";

// Instantiate the controller
$CategoryController = new CategoryC();

// Handle form submission
$list = [];
if ($_SERVER["REQUEST_METHOD"] == "POST" && isset($_POST['Category']) && isset($_POST['search'])) {
    $name = htmlspecialchars($_POST['Category']); // Get the selected category
    
    // If the selected category is empty (which means "All categories"), fetch all products
    if ($name == "") {
        $list = $CategoryController->afficheAllProduits(); // Fetch all products
    } else {
        $list = $CategoryController->afficheproduits($name); // Fetch products by selected category
    }
} else {
    $list = $CategoryController->afficheAllProduits(); // Fetch all products by default
}

// Retrieve all categories for the dropdown
$categories = $CategoryController->affichercategories();

?>

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <!-- CSS -->
    <link rel="stylesheet" href="./dist/styles.css">
    <link rel="stylesheet" href="./dist/all.css">
    <link href="https://fonts.googleapis.com/css?family=Source+Sans+Pro:400,400i,600,600i,700,700i" rel="stylesheet">
    <title>Dashboard - Gestion des Produits</title>
    

    <!-- Custom CSS  for rechrech et tri-->
    <style>
        /* General Form Styling */
        form {
            background-color: #fff;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1);
            margin-bottom: 30px;
            display: flex;
            flex-wrap: wrap;
            gap: 10px;
            justify-content: space-between;
        }

        label {
            font-weight: 600;
            color: #4a5568;
            margin-right: 10px;
        }

        /* Dropdown Styling */
        select, input[type="text"] {
            padding: 10px;
            border-radius: 5px;
            border: 1px solid #e2e8f0;
            font-size: 14px;
            color: #4a5568;
            outline: none;
            transition: border-color 0.3s ease;
        }

        select:focus, input[type="text"]:focus {
            border-color: #3182ce;
        }

        /* Button Styling */
        button, input[type="submit"] {
            background-color: #4299e1;
            color: white;
            padding: 10px 20px;
            border-radius: 5px;
            border: none;
            font-size: 14px;
            cursor: pointer;
            transition: background-color 0.3s ease;
        }

        button:hover, input[type="submit"]:hover {
            background-color: #3182ce;
        }

        .bg-red-500 {
            background-color: #f56565;
        }
        
        .bg-blue-500 {
            background-color: #4299e1;
        }

        .bg-blue-500:hover {
            background-color: #3182ce;
        }

        .bg-red-500:hover {
            background-color: #e53e3e;
        }

        /* Aligning the forms and buttons */
        .form-container {
            display: flex;
            justify-content: space-between;
            gap: 20px;
            width: 100%;
        }

        .form-container > div {
            flex: 1;
        }

        .form-container input[type="text"], .form-container select {
            width: 100%;
        }
    </style>

</head>

<body>
    
<!--Container -->
<div class="mx-auto bg-grey-lightest">
    <!--Screen-->
    <div class="min-h-screen flex flex-col">
        <!--Header Section Starts Here-->
        <header class="bg-nav">
            <div class="flex justify-between">
                <div class="p-1 mx-3 inline-flex items-center">
                    <i class="fas fa-bars pr-2 text-white" onclick="sidebarToggle()"></i>
                    <h1 class="text-white p-2"> Produits</h1>
                </div>
                
                <div class="p-1 flex flex-row items-center">

                </div>
            </div>
        </header>
        <!--/Header-->

        <div class="flex flex-1">
            <!--Sidebar-->
            <aside id="sidebar" class="bg-side-nav w-1/2 md:w-1/6 lg:w-1/6 border-r border-side-nav hidden md:block lg:block">
                <ul class="list-reset flex flex-col">
                   

                    <li class="py-3 px-2 border-b">
                            <a href="userList.php" class="text-nav-item">Users</a>
                        </li>

                        <li class="py-3 px-2 border-b">
                            <a href="forum.php" class="text-nav-item">forum</a>
                        </li>
                        <li class="py-3 px-2 border-b">
                            <a href="reclamationlist.php" class="text-nav-item">reclamation</a>
                        </li>

                        <li class="py-3 px-2 border-b">
                            <a href="dashboard.php" class="text-nav-item">Products</a>
                            <li>
                           
        <li><a href="produitList.php?page=productList" class="hover:text-gray-400">Liste des Produits</a></li>
        <li><a href="listCategories.php?page=listCategories" class="hover:text-gray-400">Liste des Categories</a></li>
        <li><a href="addProduit.php" class="hover:text-gray-400">Ajouter un Produit</a></li>
        <li><a href="addCategory.php" class="hover:text-gray-400">Ajouter une Categorie</a></li>
  
    </li>
    <li class="py-3 px-2 border-b">
                            <a href="forms.php" class="text-nav-item">orders</a>
                        </li>
                            <div class="flex h-screen">
    <div class="w-64 text-black">
    
   
                        </li>
                </ul>
            </aside>
            <!--/Sidebar-->

            <!--Main Content-->
            <div class="flex-1 p-6">
            <!-- Header -->
           

            <!-- Filtration Form -->
            <div class="mt-6">
                <h2 class="text-2xl font-bold text-gray-700 mb-4">Recherche de Produits par Catégorie</h2>
                <form action="" method="POST" class="mb-6 flex items-center gap-4">
                    <label for="Category" class="block text-lg font-semibold">Catégorie :</label>
                    <select name="Category" id="Category" class="border rounded-lg p-2 flex-1">
                        <option value="">Toutes les catégories</option>
                        <?php
                        foreach ($categories as $category) {
                            echo '<option value="' . htmlspecialchars($category['name']) . '">' . htmlspecialchars($category['name']) . '</option>';
                        }
                        ?>
                    </select>
                    
                    <button type="submit" name="search" class="px-4 py-2 bg-blue-500 text-white font-semibold rounded-lg hover:bg-blue-600">
                        Rechercher
                    </button>
                    
                    <button class="bg-red-500 text-white px-4 py-2 rounded-md hover:bg-red-600">Déconnexion</button>
                </form>
            </div>
            
            <!-- Table Listing Products -->
            <div>
                <h2 class="text-2xl font-bold text-gray-700 mb-4">Liste des Produits</h2>
                <table class="w-full border-collapse border border-gray-300">
                    <thead>
                        <tr class="bg-gray-200 text-left">
                            <th class="border border-gray-300 px-4 py-2">ID</th>
                            <th class="border border-gray-300 px-4 py-2">Nom</th>
                            <th class="border border-gray-300 px-4 py-2">Catégorie</th>
                            <th class="border border-gray-300 px-4 py-2">Prix</th>
                            <th class="border border-gray-300 px-4 py-2">Description</th>
                            <th class="border border-gray-300 px-4 py-2 text-center">Actions</th>
                        </tr>
                    </thead>
                    <tbody>
                        <?php if (!empty($list)) { ?>
                            <?php foreach ($list as $produit) { ?>
                                <tr class="hover:bg-gray-100">
                                    <td class="border border-gray-300 px-4 py-2"><?= htmlspecialchars($produit['id']) ?></td>
                                    <td class="border border-gray-300 px-4 py-2"><?= htmlspecialchars($produit['name']) ?></td>
                                    <td class="border border-gray-300 px-4 py-2"><?= htmlspecialchars($produit['category']) ?></td>
                                    <td class="border border-gray-300 px-4 py-2"><?= htmlspecialchars($produit['price']) ?> dt</td>
                                    <td class="border border-gray-300 px-4 py-2"><?= htmlspecialchars($produit['description']) ?></td>
                                    <td class="border border-gray-300 px-4 py-2 text-center">
                                        <!-- Modifier Button -->
                                        <a href="updateProduct.php?id=<?= $produit['id'] ?>"
                                           class="px-4 py-2 bg-blue-500 text-white rounded-lg hover:bg-blue-600">
                                            Modifier
                                        </a>
                                        <!-- Supprimer Button -->
                                        <a href="deleteProduct.php?id=<?= $produit['id'] ?>"
                                           class="px-4 py-2 bg-red-500 text-white rounded-lg hover:bg-red-600 ml-2"
                                           onclick="return confirm('Êtes-vous sûr de vouloir supprimer ce produit ?');">
                                            Supprimer
                                        </a>
                                    </td>
                                </tr>
                            <?php } ?>
                        <?php } else { ?>
                            <tr>
                                <td colspan="6" class="text-center py-4 text-gray-500">Aucun produit trouvé.</td>
                            </tr>
                        <?php } ?>
                    </tbody>
                </table>
            </div>
            </div>
            <!--/Main Content-->
        </div>

        <!--Footer-->
        <footer class="bg-grey-darkest text-white p-2">
            <div class="flex flex-1 mx-auto">&copy; My Design</div>
        </footer>
        <!--/Footer-->
    </div>
</div>
<script src="./main.js"></script>
</body>
</html>