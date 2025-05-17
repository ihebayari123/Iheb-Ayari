<?php
require_once '../../controller/controller.php'; // Inclure le contrôleur
require_once '../../config.php'; // Inclure la connexion à la base de données
$conn = config::getConnexion();

$orderController = new OrderController($conn);

// Gestion de la recherche
$searchTerm = isset($_GET['search']) ? $_GET['search'] : '';

// Gestion du tri
$sortBy = isset($_GET['sort_by']) ? $_GET['sort_by'] : 'id';
$sortOrder = isset($_GET['sort_order']) ? $_GET['sort_order'] : 'ASC';

// Récupérer les livraisons avec filtres et tri
$deliveries = $orderController->showDeliveriesFiltered($searchTerm, $sortBy, $sortOrder);
?>

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <!-- Css -->
    <link rel="stylesheet" href="./dist/styles.css">
    <link rel="stylesheet" href="./dist/all.css">
    <link href="https://fonts.googleapis.com/css?family=Source+Sans+Pro:400,400i,600,600i,700,700i" rel="stylesheet">
    <title>Tables | Tailwind Admin</title>
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
                    <h1 class="text-white p-2">Logo</h1>
                </div>
                <div class="p-1 flex flex-row items-center">
                    <img onclick="profileToggle()" class="inline-block h-8 w-8 rounded-full" src="dist/images/toystory.jpg" alt="">
                    <a href="#" onclick="profileToggle()" class="text-white p-2 no-underline hidden md:block lg:block">Oussema Agrebi</a>
                    <div id="ProfileDropDown" class="rounded hidden shadow-md bg-white absolute pin-t mt-12 mr-1 pin-r">
                        <ul class="list-reset">
                          <li><a href="#" class="no-underline px-4 py-2 block text-black hover:bg-grey-light">My account</a></li>
                          <li><a href="#" class="no-underline px-4 py-2 block text-black hover:bg-grey-light">Notifications</a></li>
                          <li><hr class="border-t mx-2 border-grey-ligght"></li>
                          <li><a href="#" class="no-underline px-4 py-2 block text-black hover:bg-grey-light">Logout</a></li>
                        </ul>
                    </div>
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

                        <li class="py-3 px-2 border-b">
                            <a href="forms.php" class="text-nav-item">orders</a>
                            <li>
                           
        <li><a href="formsdeliveries.php" class="hover:text-gray-400">Delivery Form</a></li>
        <li><a href="ordertables.php" class="hover:text-gray-400">Orders Tables</a></li>
        <li><a href="deliverytables.php" class="hover:text-gray-400">Deliveries table</a></li>
  
  
    </li>
    
                            <div class="flex h-screen">
    <div class="w-64 text-black">
    
   
                        </li>
                </ul>
            </aside>
            <!--/Sidebar-->
            <!--Main-->
            <main class="bg-white-500 flex-1 p-3 overflow-hidden">

                <div class="flex flex-col">
                    <!-- Card Sextion Starts Here -->
                    
                    
                    <!-- /Cards Section Ends Here -->

                    <!--Grid Form-->
                    
                    <div class="flex flex-1  flex-col md:flex-row lg:flex-row mx-2">
                        <div class="mb-2 border-solid border-gray-300 rounded border shadow-sm w-full">
                            <div class="bg-gray-200 px-2 py-3 border-solid border-gray-200 border-b">
                                Full Table
                            </div>
                            <form method="GET" action="deliverytables.php">
    <input class="appearance-none block w-full bg-grey-200 text-grey-darker border border-grey-200 rounded py-3 px-4 leading-tight focus:outline-none focus:bg-white focus:border-grey" type="text" name="search" placeholder="Rechercher..." value="<?= htmlspecialchars($searchTerm) ?>">
    <button  class="bg-blue-500 hover:bg-blue-800 text-white font-bold py-2 px-4 rounded-full" type="submit">Rechercher</button>
</form> 
                            <div class="p-3">
                                
                              
<table class="table-responsive w-full rounded">
    <thead>
        <tr>
            <th class="border w-1/4 px-4 py-2" ><a href="?sort_by=id&sort_order=<?= $sortOrder === 'ASC' ? 'DESC' : 'ASC' ?>">ID</a></th>
            <th class="border w-1/4 px-4 py-2"><a href="?sort_by=order_id&sort_order=<?= $sortOrder === 'ASC' ? 'DESC' : 'ASC' ?>">ID Commande</a></th>
            <th class="border w-1/4 px-4 py-2"><a href="?sort_by=delivery_status&sort_order=<?= $sortOrder === 'ASC' ? 'DESC' : 'ASC' ?>">Statut</a></th>
            <th class="border w-1/4 px-4 py-2"><a href="?sort_by=delivery_date&sort_order=<?= $sortOrder === 'ASC' ? 'DESC' : 'ASC' ?>">Date</a></th>
            <th class="border w-1/4 px-4 py-2">Actions</th>
        </tr>
    </thead>
    <tbody>
        <?php foreach ($deliveries as $delivery) : ?>
            <tr>
                <td class="border px-4 py-2"><?= htmlspecialchars($delivery['id']) ?></td>
                <td class="border px-4 py-2"><?= htmlspecialchars($delivery['order_id']) ?></td>
                <td class="border px-4 py-2"><?= htmlspecialchars($delivery['delivery_status']) ?></td>
                <td class="border px-4 py-2"><?= htmlspecialchars($delivery['delivery_date']) ?></td>
                <td class="border px-4 py-2">
                    <!-- Bouton Modifier -->
                    <a class="bg-teal-300 cursor-pointer rounded p-1 mx-1 text-white" href="editdeliveries.php?id=<?= $delivery['id'] ?>"><i class="fas fa-edit"></i></a>
                    <!-- Bouton Supprimer -->
                    <form action="deleteDelivery.php" method="POST" style="display:inline;">
                        <input type="hidden" name="id" value="<?= $delivery['id'] ?>">
                        <button type="submit" onclick="return confirm('Êtes-vous sûr de vouloir supprimer cette livraison ?')"><i class="fas fa-trash"></i></button>
                    </form>
                </td>
            </tr>
        <?php endforeach; ?>
    </tbody>
</table>
                            </div>
                        </div>
                    </div>
                    <!--/Grid Form-->
                </div>
            </main>
            <!--/Main-->
        </div>
        <!--Footer-->
        <footer class="bg-grey-darkest text-white p-2">
            <div class="flex flex-1 mx-auto">&copy; My Design</div>
        </footer>
        <!--/footer-->

    </div>

</div>

<script src="./main.js"></script>

</body>

</html>