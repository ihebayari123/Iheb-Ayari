<?php
include '../../Controller/CategoryController.php';

$controller = new CategoryController();
$categories = $controller->listCategories();
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
                        <h1 class="text-white p-2">list Categories</h1>
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
                        list Categories
                        </div>

                        <title>Liste des Catégories</title>
    <link href="https://cdn.jsdelivr.net/npm/tailwindcss@2.2.19/dist/tailwind.min.css" rel="stylesheet">
</head>
<body class="bg-gray-100">
    <div class="max-w-6xl mx-auto bg-white p-6 rounded-xl shadow-lg mt-10">
        <div class="flex justify-between items-center mb-6">
            <h1 class="text-3xl font-bold">Liste des Catégories</h1>
            <!-- Button to Add a New Category -->
            <a href="addCategory.php"
               class="px-6 py-3 bg-green-500 text-white font-semibold rounded-lg hover:bg-green-600 focus:outline-none focus:ring-2 focus:ring-green-400">
               + Ajouter une Catégorie
            </a>
        </div>
        
        <!-- Go to Dashboard Button -->
        <div class="flex justify-end mb-6">
            <a href="dashboard.php" 
               class="px-6 py-3 bg-indigo-600 text-white font-semibold rounded-lg hover:bg-indigo-700 focus:outline-none focus:ring-2 focus:ring-indigo-500">
               Aller au Tableau de Bord
            </a>
        </div>

        <table class="w-full border-collapse border border-gray-300">
            <thead>
                <tr class="bg-gray-200 text-left">
                    <th class="border border-gray-300 px-4 py-2">ID</th>
                    <th class="border border-gray-300 px-4 py-2">Nom</th>
                    <th class="border border-gray-300 px-4 py-2">Description</th>
                    <th class="border border-gray-300 px-4 py-2 text-center">Actions</th>
                </tr>
            </thead>
            <tbody>
                <?php foreach ($categories as $category): ?>
                <tr class="hover:bg-gray-100">
                    <td class="border border-gray-300 px-4 py-2"><?php echo htmlspecialchars($category['id']); ?></td>
                    <td class="border border-gray-300 px-4 py-2"><?php echo htmlspecialchars($category['name']); ?></td>
                    <td class="border border-gray-300 px-4 py-2"><?php echo htmlspecialchars($category['description']); ?></td>
                    <td class="border border-gray-300 px-4 py-2 text-center">
                        <!-- Modifier Button -->
                        <a href="updateCategory.php?id=<?php echo $category['id']; ?>"
                           class="px-4 py-2 bg-blue-500 text-white rounded-lg hover:bg-blue-600 focus:outline-none focus:ring-2 focus:ring-blue-400 inline-block">
                           Modifier
                        </a>
                        <!-- Supprimer Button -->
                        <a href="deleteCategory.php?id=<?php echo $category['id']; ?>"
                           class="px-4 py-2 bg-red-500 text-white rounded-lg hover:bg-red-600 focus:outline-none focus:ring-2 focus:ring-red-400 inline-block ml-2"
                           onclick="return confirm('Êtes-vous sûr de vouloir supprimer cette catégorie ?');">
                           Supprimer
                        </a>
                    </td>
                </tr>
                <?php endforeach; ?>
            </tbody>
        </table>
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


