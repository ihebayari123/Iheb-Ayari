<?php
include '../../Controller/CategoryController.php';
include_once '../../Model/CategoryModel.php';

$errorMessage = null;
$successMessage = null;
$category = null;

// Create an instance of the CategoryController
$controller = new CategoryController();

// Retrieve the category
if (isset($_GET['id']) && is_numeric($_GET['id'])) {
    $id = intval($_GET['id']);
    $category = $controller->getCategory($id);

    if ($category === null) {
        $errorMessage = "Catégorie non trouvée.";
    }
}

// Handle form submission
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    if (isset($_POST['id'], $_POST['name'], $_POST['description']) && !empty(trim($_POST['name']))) {
        try {
            // Input Validation
            $name = htmlspecialchars(trim($_POST['name']));
            $description = htmlspecialchars(trim($_POST['description']));

            if (!preg_match('/^[a-zA-Z\s]+$/', $name)) {
                throw new Exception('Le nom de la catégorie doit contenir uniquement des lettres et des espaces.');
            }

            if (strlen($description) < 5) {
                throw new Exception('La description doit contenir au moins 5 caractères.');
            }

            // Update the Category object
            $category->setName($name);
            $category->setDescription($description);

            // Update in the database
            $result = $controller->updateCategory($category);

            if (isset($result['success'])) {
                header('Location: listCategories.php?message=Catégorie modifiée avec succès.');
                exit();
            } else {
                $errorMessage = $result['error'] ?? 'Une erreur est survenue.';
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
                        <h1 class="text-white p-2">update Category</h1>
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
                        update Category
                        </div>

                        <title>Modifier une Catégorie</title>
    <link href="https://cdn.jsdelivr.net/npm/tailwindcss@2.2.19/dist/tailwind.min.css" rel="stylesheet">
    <script type="text/javascript">
        function validateForm() {
            const name = document.getElementById("name").value.trim();
            const description = document.getElementById("description").value.trim();

            // Check if name is valid
            const namePattern = /^[a-zA-Z\s]+$/;
            if (!namePattern.test(name)) {
                alert("Le nom de la catégorie doit contenir uniquement des lettres et des espaces.");
                return false;
            }

            // Check if description has a minimum length
            if (description.length < 5) {
                alert("La description doit contenir au moins 5 caractères.");
                return false;
            }

            return true;
        }
    </script>
</head>
<body class="bg-gradient-to-r from-green-400 to-blue-600 font-sans">
    <div class="max-w-4xl mx-auto bg-white p-8 rounded-xl shadow-xl mt-10">
        <h2 class="text-4xl font-bold text-gray-800 text-center mb-6">Modifier une Catégorie</h2>

        <?php if ($errorMessage): ?>
            <div class="bg-red-100 text-red-700 border border-red-400 p-4 rounded mb-6">
                <strong>Erreur !</strong> <?php echo htmlspecialchars($errorMessage); ?>
            </div>
        <?php elseif ($category): ?>
            <form action="" method="POST" onsubmit="return validateForm()">
                <input type="hidden" name="id" value="<?php echo htmlspecialchars($category->getId()); ?>">

                <div class="mb-6">
                    <label for="name" class="block text-lg font-semibold text-gray-800">Nom de la catégorie</label>
                    <input type="text" id="name" name="name" value="<?php echo htmlspecialchars($category->getName()); ?>" 
                           class="w-full p-4 mt-2 border-2 border-gray-300 rounded-lg focus:ring-2 focus:ring-indigo-500" required>
                </div>

                <div class="mb-6">
                    <label for="description" class="block text-lg font-semibold text-gray-800">Description</label>
                    <textarea id="description" name="description" class="w-full p-4 mt-2 border-2 border-gray-300 rounded-lg focus:ring-2 focus:ring-indigo-500" required><?php echo htmlspecialchars($category->getDescription()); ?></textarea>
                </div>

                <div class="flex justify-between">
                    <button type="submit" class="px-6 py-3 bg-indigo-600 text-white font-semibold rounded-lg hover:bg-indigo-700 focus:outline-none focus:ring-2 focus:ring-indigo-500">
                        Modifier
                    </button>
                    <button type="button" onclick="window.location.href='listCategories.php';" 
                            class="px-6 py-3 bg-gray-300 text-black font-semibold rounded-lg hover:bg-gray-400 focus:outline-none">
                        Annuler
                    </button>
                </div>
            </form>
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


