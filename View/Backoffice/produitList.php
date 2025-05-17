<?php
include '../../Controller/ProductController.php';

$productController = new ProductController();
$list = $productController->listProducts();
// Check if a product was deleted and show a success message
if (isset($_GET['status']) && $_GET['status'] == 'deleted') {
    $successMessage = "Le produit a été supprimé avec succès.";
}
// Check if there is a status message to display
$status = $_GET['status'] ?? '';
?>

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>List of Users</title>
    <link href="assets/vendor/bootstrap/css/bootstrap.min.css" rel="stylesheet">
    <link href="assets/vendor/bootstrap-icons/bootstrap-icons.css" rel="stylesheet">
    <link href="https://fonts.googleapis.com/css?family=Source+Sans+Pro:400,400i,600,600i,700,700i" rel="stylesheet">
    <link href="assets/css/main.css" rel="stylesheet">
    <link rel="stylesheet" href="./dist/styles.css">
    <link rel="stylesheet" href="./dist/all.css">
</head>

<body>
    <div class="mx-auto bg-grey-lightest">
        <div class="min-h-screen flex flex-col">
            <!-- Header -->
            <header class="bg-nav">
                <div class="flex justify-between">
                    <div class="p-1 mx-3 inline-flex items-center">
                        <i class="fas fa-bars pr-2 text-white"></i>
                        <h1 class="text-white p-2">produit List</h1>
                    </div>
                    <div class="p-1 flex flex-row items-center">
                       
                        
                    </div>
                </div>
            </header>
            <!-- /Header -->

            <div class="flex flex-1">
                <!-- Sidebar -->
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
                        </li>
                        <li class="py-3 px-2 border-b">
                            <a href="forms.php" class="text-nav-item">orders</a>
                        </li>
                </ul>
            </aside>
                <!-- /Sidebar -->

                <main class="flex flex-1 flex-col md:flex-row lg:flex-row mx-2">
                    <div class="mb-2 border-solid border-gray-300 rounded border shadow-sm w-full">
                        <div class="bg-gray-200 px-2 py-3 border-b">
                        produit List
                        </div>

                        <title>Liste des Produits</title>
    <link href="https://cdn.jsdelivr.net/npm/tailwindcss@2.2.19/dist/tailwind.min.css" rel="stylesheet">
</head>
<body class="bg-gray-100 font-sans leading-relaxed">

    <!-- Dashboard Container -->
    <div class="min-h-screen flex flex-col">

        

        <!-- Main Content -->
        <div class="container mx-auto p-6">

            <!-- Product List Section -->
            <div class="bg-white p-6 rounded-lg shadow-md">

                <!-- Success Message -->
                <?php if ($status === 'updated'): ?>
                    <div class="text-green-500 mb-4">Produit mis à jour avec succès!</div>
                <?php elseif (isset($successMessage)): ?>
                    <div class="text-green-500 mb-4"><?php echo $successMessage; ?></div>
                <?php endif; ?>

                <h2 class="text-2xl font-semibold text-gray-800 mb-6">Liste des Produits</h2>

                <!-- Add New Product Button -->
                <a href="addProduit.php" class="px-6 py-3 bg-green-600 text-white rounded-lg shadow-lg hover:bg-green-700 mb-4 inline-block">
                    Ajouter un nouveau produit
                </a>

                <!-- Go to Dashboard Button -->
                <div class="flex justify-end mb-6">
                    <a href="dashboard.php" 
                       class="px-6 py-3 bg-indigo-600 text-white font-semibold rounded-lg hover:bg-indigo-700 focus:outline-none focus:ring-2 focus:ring-indigo-500">
                       Aller au Tableau de Bord
                    </a>
                </div>

                <!-- Table -->
                <table class="min-w-full table-auto border-collapse border border-gray-200">
                    <thead class="bg-gray-100">
                        <tr>
                            <th class="border-b px-4 py-2 text-left text-sm font-semibold text-gray-600">ID</th>
                            <th class="border-b px-4 py-2 text-left text-sm font-semibold text-gray-600">Nom</th>
                            <th class="border-b px-4 py-2 text-left text-sm font-semibold text-gray-600">Prix</th>
                            <th class="border-b px-4 py-2 text-left text-sm font-semibold text-gray-600">Catégorie</th>
                            <th class="border-b px-4 py-2 text-left text-sm font-semibold text-gray-600">Actions</th>
                        </tr>
                    </thead>
                    <tbody class="text-sm text-gray-700">
                        <?php foreach ($list as $product): ?>
                            <tr class="hover:bg-gray-50">
                                 <td class="border-b px-4 py-3"><?php echo htmlspecialchars($product['id']); ?></td>
                                 <td class="border-b px-4 py-3"><?php echo htmlspecialchars($product['name']); ?></td>
                                 <td class="border-b px-4 py-3"><?php echo htmlspecialchars($product['price']); ?> USD</td>
                                 <td class="border-b px-4 py-3"><?php echo htmlspecialchars($product['category']); ?></td>
                                 <td class="border-b px-4 py-3 flex items-center space-x-4">
                                    <!-- Modify link -->
                                    <a href="updateProduct.php?id=<?php echo $product['id']; ?>" class="text-blue-500 hover:text-blue-700 text-sm">Modifier</a>
                                    <!-- Delete link -->
                                    <a href="deleteProduct.php?id=<?php echo $product['id']; ?>" class="text-red-500 hover:text-red-700 text-sm">Supprimer</a>
                                </td>
                            </tr>
                        <?php endforeach; ?>
                    </tbody>
                </table>
            </div>

        </div>
    </div>
                    </div>
                </main>
            </div>

            <!-- Footer -->
            <footer class="bg-grey-darkest text-white p-2">
                <div class="flex flex-1 mx-auto">&copy; AgriCulture</div>
            </footer>
        </div>
    </div>
</body>

</html>

