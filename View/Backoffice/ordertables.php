<?php
require_once '../../controller/controller.php';  // Assurez-vous que le contrôleur est bien inclus
require_once '../../config.php';  // Inclure la connexion à la base de données
$conn = config::getConnexion();
$orderController = new OrderController($conn);  // Instancier le contrôleur
$orders = $orderController->showOrders();  // Récupérer toutes les commandes
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
                            <div class="p-3">
                                
                                <table class="table-responsive w-full rounded">
    <tr>
        <th class="border w-1/4 px-4 py-2">ID</th>
        <th class="border w-1/4 px-4 py-2">Nom du client</th>
        <th class="border w-1/4 px-4 py-2">Email</th>
        <th class="border w-1/4 px-4 py-2">Produit</th>
        <th class="border w-1/4 px-4 py-2">Quantité</th>
        <th class="border w-1/4 px-4 py-2">Prix</th>
        <th class="border w-1/4 px-4 py-2">Actions</th>
    </tr>
    <?php foreach ($orders as $order): ?>
        <tr>
            <td class="border px-4 py-2"><?php echo $order['id']; ?></td>
            <td class="border px-4 py-2"><?php echo $order['customer_name']; ?></td>
            <td class="border px-4 py-2"><?php echo $order['customer_email']; ?></td>
            <td class="border px-4 py-2"><?php echo $order['product_name']; ?></td>
            <td class="border px-4 py-2"><?php echo $order['quantity']; ?></td>
            <td class="border px-4 py-2"><?php echo $order['price']; ?></td>
            <td class="border px-4 py-2">
                <a class="bg-teal-300 cursor-pointer rounded p-1 mx-1 text-white" href="updateOrder.php?id=<?php echo $order['id']; ?>"><i class="fas fa-edit"></i></a> | 
                <a class="bg-teal-300 cursor-pointer rounded p-1 mx-1 text-red-500" href="deleteOrder.php?id=<?php echo $order['id']; ?>"><i class="fas fa-trash"></i></a>
            </td>
        </tr>
    <?php endforeach; ?>
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