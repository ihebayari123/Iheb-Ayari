<?php
include '../../Controller/CategoryController.php';
include_once '../../Model/CategoryModel.php';

$errorMessage = null;

// Check if the form is submitted
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    if (
        isset($_POST['name'], $_POST['description']) &&
        !empty(trim($_POST['name'])) &&
        !empty(trim($_POST['description']))
    ) {
        try {
            // Create a new Category object
            $category = new Category(
                null, // ID is auto-generated
                htmlspecialchars(trim($_POST['name'])),
                htmlspecialchars(trim($_POST['description']))
            );

            // Instantiate the CategoryController and add the category
            $categoryController = new CategoryController();
            $result = $categoryController->addCategory($category);

            // Handle success or errors from the controller
            if (isset($result['success'])) {
                // Redirect to the category list page with success message
                header('Location: listCategories.php?message=Catégorie ajoutée avec succès');
                exit();
            } else {
                // Handle errors
                $errorMessage = implode('<br>', $result['errors'] ?? ['Une erreur inattendue est survenue.']);
            }
        } catch (Exception $e) {
            $errorMessage = $e->getMessage();
        }
    } else {
        // If not all fields are filled, show an error message
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
                        <h1 class="text-white p-2">add category</h1>
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
                        add category
                        </div>
                        <script type="text/javascript">
        function validateForm() {
            // Clear previous error messages
            document.getElementById('nameError').textContent = '';
            document.getElementById('descriptionError').textContent = '';

            const name = document.forms["categoryForm"]["name"].value.trim();
            const description = document.forms["categoryForm"]["description"].value.trim();

            let isValid = true;

            // Verify that all fields are filled
            if (!name || !description) {
                if (!name) {
                    document.getElementById('nameError').textContent = 'Le nom de la catégorie est requis.';
                }
                if (!description) {
                    document.getElementById('descriptionError').textContent = 'La description est requise.';
                }
                isValid = false;
            }

            // Verify that the name contains only letters and spaces
            const namePattern = /^[a-zA-Z\s]+$/;
            if (name && !namePattern.test(name)) {
                document.getElementById('nameError').textContent = 'Le nom de la catégorie doit contenir uniquement des lettres et des espaces.';
                isValid = false;
            }

            // Verify that the description contains only letters and spaces
            const descriptionPattern = /^[a-zA-Z\s]+$/;
            if (description && !descriptionPattern.test(description)) {
                document.getElementById('descriptionError').textContent = 'La description doit contenir uniquement des lettres et des espaces.';
                isValid = false;
            }

            return isValid;
        }
    </script>
</head>
<body class="bg-gradient-to-r from-green-400 to-blue-500 font-sans">

    <div class="max-w-4xl mx-auto bg-white p-8 rounded-xl shadow-xl mt-10">
        <h2 class="text-4xl font-bold text-gray-800 text-center mb-6">Ajouter une Catégorie</h2>

        <!-- Error Message -->
        <?php if (!empty($errorMessage)): ?>
            <div class="bg-red-100 text-red-700 border border-red-400 p-4 rounded mb-6">
                <strong>Erreur !</strong> <?php echo htmlspecialchars($errorMessage); ?>
            </div>
        <?php endif; ?>

        <!-- Form -->
        <form name="categoryForm" action="" method="POST" onsubmit="return validateForm()">

            <!-- Nom de la catégorie -->
            <div class="mb-6">
                <label for="name" class="block text-lg font-semibold text-gray-800">Nom de la catégorie</label>
                <input type="text" id="name" name="name" value="<?php echo isset($_POST['name']) ? htmlspecialchars($_POST['name']) : ''; ?>"
                       class="w-full p-4 mt-2 border-2 border-gray-300 rounded-lg focus:ring-2 focus:ring-blue-500 focus:outline-none" required>
                <span id="nameError" class="text-red-600 text-sm"></span>
            </div>

            <!-- Description -->
            <div class="mb-6">
                <label for="description" class="block text-lg font-semibold text-gray-800">Description</label>
                <textarea id="description" name="description" class="w-full p-4 mt-2 border-2 border-gray-300 rounded-lg focus:ring-2 focus:ring-blue-500 focus:outline-none" required><?php echo isset($_POST['description']) ? htmlspecialchars($_POST['description']) : ''; ?></textarea>
                <span id="descriptionError" class="text-red-600 text-sm"></span>
            </div>

            <!-- Boutons -->
            <div class="flex justify-between">
                <!-- Ajouter Button -->
                <button 
                    type="submit"
                    class="px-8 py-3 bg-indigo-600 text-white font-semibold rounded-lg hover:bg-indigo-700 focus:outline-none focus:ring-2 focus:ring-indigo-500">
                    Ajouter
                </button>
                <!-- Réinitialiser Button -->
                <button 
                    type="reset" 
                    onclick="window.location.href='listCategories.php';" 
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


