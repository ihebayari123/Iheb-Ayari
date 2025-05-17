<?php
include '../../Controller/ProductController.php';
include_once '../../Model/ProductModel.php';

$errorMessage = null;

// Check if the form is submitted
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    if (
        isset($_POST['name'], $_POST['description'], $_POST['price'], $_POST['availability'], $_POST['category']) &&
        !empty(trim($_POST['name'])) &&
        !empty(trim($_POST['description'])) &&
        !empty(trim($_POST['price'])) &&
        !empty(trim($_POST['category']))
    ) {
        try {
            // Sanitize and cast price to float
            $price = filter_var($_POST['price'], FILTER_VALIDATE_FLOAT);
            if ($price === false || $price <= 0) {
                throw new Exception('Le prix doit être un nombre valide et positif.');
            }

            
            // Create a new Product object (no ID is passed since it's auto-generated)
            $product = new Product(null,
                htmlspecialchars(trim($_POST['name'])),
                htmlspecialchars(trim($_POST['description'])),
                $price,  // Pass the sanitized price as float
                isset($_POST['availability']) ? true : false, // Ensure availability is a boolean
                htmlspecialchars(trim($_POST['category'])),
            );

            // Instantiate the ProductController and add the product
            $productController = new ProductController();
            $result = $productController->addProduct($product);

            // Handle success or errors from the controller
            if (isset($result['success'])) {
                // Redirect to the product list page with success message
                header('Location: produitList.php?message=Product+added+successfully');
                exit();
            } else {
                // Handle the case when there are validation errors or any other issues
                $errorMessage = implode('<br>', $result['errors'] ?? ['An unexpected error occurred.']);
            }
        } catch (Exception $e) {
             // Catch any exceptions and display the error message
            $errorMessage = $e->getMessage();
        }
    } else {
        // If not all fields are filled in, show an error message
        $errorMessage = "Tous les champs doivent être remplis !";
    }
}
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
                        <h1 class="text-white p-2">Ajouter un produit</h1>
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
                            add produit
                        </div>

                        <title>Ajouter un Produit</title>
    <link href="https://cdn.jsdelivr.net/npm/tailwindcss@2.2.19/dist/tailwind.min.css" rel="stylesheet">
    <script type="text/javascript">
        function validateForm() {
            const name = document.forms["productForm"]["name"].value.trim();
            const description = document.forms["productForm"]["description"].value.trim();
            const price = document.forms["productForm"]["price"].value.trim();
            const category = document.forms["productForm"]["category"].value.trim();

            // Vérifier que tous les champs sont remplis
            if (!name || !description || !price || !category) {
                alert("Tous les champs doivent être remplis !");
                return false;
            }

            // Vérifier que le nom ne contient que des lettres et des espaces
            const namePattern = /^[a-zA-Z\s]+$/;
            if (!namePattern.test(name)) {
                alert("Le nom du produit doit contenir uniquement des lettres et des espaces !");
                return false;
            }

            // Vérifier que le prix est un nombre positif
            if (isNaN(price) || parseFloat(price) <= 0) {
                alert("Le prix doit être un nombre positif !");
                return false;
            }

            // Vérifier que la catégorie ne contient que des lettres et des espaces
            const categoryPattern = /^[a-zA-Z\s]+$/;
            if (!categoryPattern.test(category)) {
                alert("La catégorie doit contenir uniquement des lettres et des espaces !");
                return false;
            }

            return true;
        }
    </script>
</head>
<body class="bg-gradient-to-r from-purple-500 to-indigo-600 font-sans">

    <div class="max-w-4xl mx-auto bg-white p-8 rounded-xl shadow-xl mt-10">
        <h2 class="text-4xl font-bold text-gray-800 text-center mb-6">Ajouter un Produit</h2>

        <!-- Message d'erreur -->
        <?php if (!empty($errorMessage)): ?>
            <div class="bg-red-100 text-red-700 border border-red-400 p-4 rounded mb-6">
                <strong>Erreur !</strong> <?php echo htmlspecialchars($errorMessage); ?>
            </div>
        <?php endif; ?>

        <!-- Formulaire -->
        <form name="productForm" action="" method="POST" enctype="multipart/form-data" onsubmit="return validateForm()">
            
            <!-- Nom du produit -->
            <div class="mb-6">
                <label for="name" class="block text-lg font-semibold text-gray-800">Nom du produit</label>
                <input type="text" id="name" name="name" value="<?php echo isset($_POST['name']) ? htmlspecialchars($_POST['name']) : ''; ?>"
                       class="w-full p-4 mt-2 border-2 border-gray-300 rounded-lg focus:ring-2 focus:ring-indigo-500 focus:outline-none" >
            </div>

            <!-- Description -->
            <div class="mb-6">
                <label for="description" class="block text-lg font-semibold text-gray-800">Description</label>
                <textarea id="description" name="description" class="w-full p-4 mt-2 border-2 border-gray-300 rounded-lg focus:ring-2 focus:ring-indigo-500 focus:outline-none" required><?php echo isset($_POST['description']) ? htmlspecialchars($_POST['description']) : ''; ?></textarea>
            </div>

            <!-- Prix -->
            <div class="mb-6">
                <label for="price" class="block text-lg font-semibold text-gray-800">Prix </label>
                <input type="number" id="price" name="price" step="0.01" value="<?php echo isset($_POST['price']) ? htmlspecialchars($_POST['price']) : ''; ?>"
                       class="w-full p-4 mt-2 border-2 border-gray-300 rounded-lg focus:ring-2 focus:ring-indigo-500 focus:outline-none" >
            </div>

            <!-- Disponibilité -->
            <div class="mb-6 flex items-center">
                <input type="checkbox" name="availability" id="availability" value="1" <?php echo isset($_POST['availability']) ? 'checked' : ''; ?>
                       class="form-checkbox h-6 w-6 text-indigo-600 focus:ring-2 focus:ring-indigo-500">
                <label for="availability" class="ml-2 text-lg font-semibold text-gray-800">Disponible</label>
            </div>

            <!-- Catégorie -->
            <div class="mb-6">
                <label for="category" class="block text-lg font-semibold text-gray-800">Catégorie</label>
                <input type="text" id="category" name="category" value="<?php echo isset($_POST['category']) ? htmlspecialchars($_POST['category']) : ''; ?>"
                       class="w-full p-4 mt-2 border-2 border-gray-300 rounded-lg focus:ring-2 focus:ring-indigo-500 focus:outline-none" >
            </div>

            <!-- Buttons -->
            <div class="flex justify-between">
                <button type="submit" class="px-8 py-3 bg-indigo-600 text-white font-semibold rounded-lg hover:bg-indigo-700 focus:outline-none focus:ring-2 focus:ring-indigo-500">
                    Ajouter
                    
                </button>
                <button type="reset" 
                    onclick="window.location.href='produitList.php';" 
                    class="px-8 py-3 bg-gray-300 text-black font-semibold rounded-lg hover:bg-gray-400 focus:outline-none focus:ring-2 focus:ring-gray-500">
                 Réinitialiser
                </button>
            </div>
        </form>
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