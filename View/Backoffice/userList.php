<?php
include '../../Controller/userController.php';

// Récupérer les utilisateurs en fonction du critère de recherche et de tri
$userC = new userController();

// Définir les variables pour la recherche et le tri
$searchTerm = isset($_GET['search']) ? htmlspecialchars(trim($_GET['search'])) : '';
$sortBy = isset($_GET['sort_by']) ? $_GET['sort_by'] : null;
$sortOrder = isset($_GET['sort_order']) ? $_GET['sort_order'] : 'ASC';

// Si une recherche est effectuée
if ($searchTerm) {
    $list = $userC->searchUsersByName($searchTerm);
} else {
    // Sinon, on récupère les utilisateurs triés
    $list = $userC->listUsersSorted($sortBy, $sortOrder);
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
                        <h1 class="text-white p-2">List of Users</h1>
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
                            Users Table
                        </div>

                        <!-- Formulaire de recherche et tri -->
                        <form method="GET" action="userList.php" class="flex mb-4">
                            <input type="text" name="search" placeholder="Rechercher un nom..." value="<?= isset($_GET['search']) ? htmlspecialchars($_GET['search']) : '' ?>" class="border p-2 w-full rounded">
                            <button type="submit" class="bg-blue-500 text-white px-4 py-2 rounded ml-2">Rechercher</button>

                            <!-- Tri par critère -->
                            <select name="sort_by" class="border p-2 ml-2 rounded">
                                <option value="">Trier par</option>
                                <option value="id" <?= isset($_GET['sort_by']) && $_GET['sort_by'] == 'id' ? 'selected' : '' ?>>ID</option>
                                <option value="Name" <?= isset($_GET['sort_by']) && $_GET['sort_by'] == 'Name' ? 'selected' : '' ?>>Nom</option>
                                <option value="surname" <?= isset($_GET['sort_by']) && $_GET['sort_by'] == 'surname' ? 'selected' : '' ?>>Prénom</option>
                                <option value="role" <?= isset($_GET['sort_by']) && $_GET['sort_by'] == 'role' ? 'selected' : '' ?>>Rôle</option>
                            </select>

                            <!-- Ordre de tri -->
                            <select name="sort_order" class="border p-2 ml-2 rounded">
                                <option value="ASC" <?= isset($_GET['sort_order']) && $_GET['sort_order'] == 'ASC' ? 'selected' : '' ?>>Croissant</option>
                                <option value="DESC" <?= isset($_GET['sort_order']) && $_GET['sort_order'] == 'DESC' ? 'selected' : '' ?>>Décroissant</option>
                            </select>

                            <button type="submit" class="bg-blue-500 text-white px-4 py-2 rounded ml-2">Trier</button>
                        </form>

                        <a href="adduser.php">
                            <button type="button" class="button">
                                <span class="button__text">Add User</span>
                                <span class="button__icon">
                                    <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" fill="none" stroke="currentColor" class="svg">
                                        <line x1="12" x2="12" y1="5" y2="19"></line>
                                        <line x1="5" x2="19" y1="12" y2="12"></line>
                                    </svg>
                                </span>
                            </button>
                        </a>

                        <div class="p-3">
                            <table class="table-responsive w-full rounded">
                                <thead>
                                    <tr>
                                        <th class="border w-1/7 px-4 py-2">ID</th>
                                        <th class="border w-1/4 px-4 py-2">Name</th>
                                        <th class="border w-1/6 px-4 py-2">Surname</th>
                                        <th class="border w-1/6 px-4 py-2">Email</th>
                                        <th class="border w-1/6 px-4 py-2">Role</th>
                                        <th class="border w-1/6 px-4 py-2">Actions</th>
                                    </tr>
                                </thead>
                                <tbody>
                                    <?php if (empty($list)): ?>
                                        <tr>
                                            <td colspan="7" class="text-center py-4">Aucun utilisateur trouvé</td>
                                        </tr>
                                    <?php else: ?>
                                        <?php foreach ($list as $user): ?>
                                            <tr>
                                                <td class="border px-4 py-2"><?= $user['id'] ?></td>
                                                <td class="border px-4 py-2"><?= htmlspecialchars($user['Name']) ?></td>
                                                <td class="border px-4 py-2"><?= htmlspecialchars($user['surname']) ?></td>
                                                <td class="border px-4 py-2"><?= htmlspecialchars($user['Email']) ?></td>
                                                <td class="border px-4 py-2"><?= htmlspecialchars($user['role']) ?></td>
                                                <td class="border px-4 py-2">
                                                    <a href="updateuser.php?id=<?= $user['id'] ?>" class="bg-teal-300 cursor-pointer rounded p-1 mx-1 text-white">
                                                        <i class="fas fa-edit"></i>
                                                    </a>
                                                    <a href="deleteuser.php?id=<?= $user['id'] ?>" class="bg-teal-300 cursor-pointer rounded p-1 mx-1 text-red-500" onclick="return confirm('Are you sure?')">
                                                        <i class="fas fa-trash"></i>
                                                    </a>
                                                </td>
                                            </tr>
                                        <?php endforeach; ?>
                                    <?php endif; ?>
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
