<?php
include '../../Controller/ProductController.php';
include_once '../../Model/ProductModel.php';

$errorMessage = null;
$successMessage = null;
$Product = null;

// Create an instance of the ProductController
$ProductController = new ProductController();

// Retrieve the product
if (isset($_GET['id']) && is_numeric($_GET['id'])) {
    $id = intval($_GET['id']);
    $Product = $ProductController->getProduct($id);

    if ($Product === null) {
        $errorMessage = "Produit non trouvé.";
    }
}

// Handle form submission
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    if (
        isset($_POST['id'], $_POST['name'], $_POST['description'], $_POST['price'], $_POST['category']) &&
        !empty(trim($_POST['name'])) &&
        !empty(trim($_POST['description'])) &&
        !empty(trim($_POST['price'])) &&
        !empty(trim($_POST['category']))
    ) {
        try {
            // Sanitize and validate input
            $id = intval($_POST['id']);
            $name = htmlspecialchars(trim($_POST['name']));
            $description = htmlspecialchars(trim($_POST['description']));
            $price = filter_var($_POST['price'], FILTER_VALIDATE_FLOAT);
            $category = htmlspecialchars(trim($_POST['category']));
            $availability = isset($_POST['availability']) ? 1 : 0;

            if ($price === false || $price <= 0) {
                throw new Exception('Le prix doit être un nombre valide et positif.');
            }

            // Create a Product object
            $updatedProduct = new Product($id, $name, $description, $price, $availability, $category);

            // Call updateProduct
            $result = $ProductController->updateProduct($updatedProduct);

            // Handle success or errors
            if (isset($result['success'])) {
                header('Location: produitList.php?message=Produit+modifié+avec+succès');
                exit();
            } else {
                $errorMessage = implode('<br>', $result['errors'] ?? ['Une erreur est survenue.']);
            }
        } catch (Exception $e) {
            $errorMessage = $e->getMessage();
        }
    } else {
        $errorMessage = "Tous les champs doivent être remplis.";
    }
}
?>

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>update Product</title>
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
                        <h1 class="text-white p-2">update Product</h1>
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
                        update Product
                        </div>

                        <div class="max-w-4xl mx-auto bg-white p-8 rounded-xl shadow-xl mt-10">
        <h2 class="text-4xl font-bold text-gray-800 text-center mb-6">Modifier un Produit</h2>

        <?php if ($errorMessage): ?>
            <div class="bg-red-100 text-red-700 border border-red-400 p-4 rounded mb-6">
                <strong>Erreur !</strong> <?php echo htmlspecialchars($errorMessage); ?>
            </div>
        <?php endif; ?>

        <?php if ($Product): ?>
            <form name="productForm" action="" method="POST">
                <input type="hidden" name="id" value="<?php echo htmlspecialchars($Product->getId()); ?>">

                <div class="mb-6">
                    <label for="name" class="block text-lg font-semibold text-gray-800">Nom du produit</label>
                    <input type="text" id="name" name="name" value="<?php echo htmlspecialchars($Product->getName()); ?>"
                           class="w-full p-4 mt-2 border-2 border-gray-300 rounded-lg focus:ring-2 focus:ring-indigo-500 focus:outline-none" required>
                </div>

                <div class="mb-6">
                    <label for="description" class="block text-lg font-semibold text-gray-800">Description</label>
                    <textarea id="description" name="description" class="w-full p-4 mt-2 border-2 border-gray-300 rounded-lg focus:ring-2 focus:ring-indigo-500 focus:outline-none" required><?php echo htmlspecialchars($Product->getDescription()); ?></textarea>
                </div>

                <div class="mb-6">
                    <label for="price" class="block text-lg font-semibold text-gray-800">Prix (USD)</label>
                    <input type="number" id="price" name="price" step="0.01" value="<?php echo htmlspecialchars($Product->getPrice()); ?>"
                           class="w-full p-4 mt-2 border-2 border-gray-300 rounded-lg focus:ring-2 focus:ring-indigo-500 focus:outline-none" required>
                </div>

                <div class="mb-6 flex items-center">
                    <input type="checkbox" name="availability" id="availability" value="1" <?php echo $Product->getAvailability() ? 'checked' : ''; ?>
                           class="form-checkbox h-6 w-6 text-indigo-600 focus:ring-2 focus:ring-indigo-500">
                    <label for="availability" class="ml-2 text-lg font-semibold text-gray-800">Disponible</label>
                </div>

                <div class="mb-6">
                    <label for="category" class="block text-lg font-semibold text-gray-800">Catégorie</label>
                    <input type="text" id="category" name="category" value="<?php echo htmlspecialchars($Product->getCategory()); ?>"
                           class="w-full p-4 mt-2 border-2 border-gray-300 rounded-lg focus:ring-2 focus:ring-indigo-500 focus:outline-none" required>
                </div>

                <div class="flex justify-between">
                    <button type="submit" class="px-8 py-3 bg-indigo-600 text-white font-semibold rounded-lg hover:bg-indigo-700 focus:outline-none focus:ring-2 focus:ring-indigo-500">
                        Modifier
                    </button>
                    <button type="button" onclick="window.location.href='produitList.php';" 
                            class="px-8 py-3 bg-gray-300 text-black font-semibold rounded-lg hover:bg-gray-400 focus:outline-none focus:ring-2 focus:ring-gray-500">
                        Annuler
                    </button>
                </div>
            </form>
        <?php else: ?>
            <div class="bg-yellow-100 text-yellow-700 border border-yellow-400 p-4 rounded">
                <strong>Erreur !</strong> Aucun produit trouvé pour l'ID donné.
            </div>
        <?php endif; ?>
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


