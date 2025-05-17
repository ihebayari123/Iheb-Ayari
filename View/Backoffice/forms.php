<?php
require_once '../../controller/controller.php';  // Assurez-vous que le contrôleur est bien inclus
require_once '../../config.php';  // Inclure la connexion à la base de données

// Crée une instance de la connexion à la base de données
// Récupérez la connexion via la classe config
$conn = config::getConnexion();

// Créez une instance du contrôleur avec la connexion
$orderController = new OrderController($conn);

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Récupère les données du formulaire
    $customerName = $_POST['customer_name'];
    $email = $_POST['customer_email'];
    $productName = $_POST['product_name'];
    $quantity = $_POST['quantity'];
    $price = $_POST['price'];

    // Ajoute la commande via le contrôleur
    if ($orderController->addOrder($customerName, $email, $productName, $quantity, $price)) {
        // Affiche le message uniquement après que la commande soit ajoutée
        echo "Commande ajoutée avec succès!";
    } else {
        echo "Erreur lors de l'ajout de la commande.";
    }
}
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
    <title>Forms | Tailwind Admin</title>
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
                    <img onclick="profileToggle()" class="inline-block h-8 w-8 rounded-full" src="https://avatars0.githubusercontent.com/u/4323180?s=460&v=4" alt="">
                    <a href="#" onclick="profileToggle()" class="text-white p-2 no-underline hidden md:block lg:block">Adam Wathan</a>
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
                    <div class="flex flex-1  flex-col md:flex-row lg:flex-row mx-2">
                        <!--Horizontal form-->
                        
                        <!--/Horizontal form-->

                        <!--Underline form-->
                        
                        <!--/Underline form-->
                    </div>
                    <!-- /Cards Section Ends Here -->

                    <!--Grid Form-->

                    <div class="flex flex-1  flex-col md:flex-row lg:flex-row mx-2">
                        <div class="mb-2 border-solid border-gray-300 rounded border shadow-sm w-full">
                            <div class="bg-gray-200 px-2 py-3 border-solid border-gray-200 border-b">
                                Form Grid
                            </div>
                            <div class="p-3">
                                <form class="w-full" action="forms.php" method="POST" onsubmit="return validateOrderForm();">
                                    <div class="flex flex-wrap -mx-3 mb-6">
                                        <div class="w-full md:w-1/2 px-3 mb-6 md:mb-0">
                                            <label class="block uppercase tracking-wide text-gray-700 text-xs font-light mb-1"
                                                   for="grid-first-name">
                                                   Nom du client
                                            </label>
                                            <input class="appearance-none block w-full bg-gray-200 text-gray-700 border  rounded py-3 px-4 mb-3 leading-tight focus:outline-none focus:bg-white-500"
                                            name="customer_name" id="customer_name" type="text" placeholder="Jane">
                                            
                                        </div>
                                        <div class="w-full md:w-1/2 px-3">
                                            <label class="block uppercase tracking-wide text-gray-700 text-xs font-light mb-1"
                                                   for="grid-last-name">
                                                Email
                                            </label>
                                            <input class="appearance-none block w-full bg-gray-200 text-grey-darker border border-gray-200 rounded py-3 px-4 leading-tight focus:outline-none focus:bg-white-500 focus:border-gray-600"
                                            name="customer_email" id="customer_email" type="email" placeholder="abcd@gmail.com">
                                        </div>
                                    </div>
                                    <div class="flex flex-wrap -mx-3 mb-6">
                                        <div class="w-full px-3">
                                            <label class="block uppercase tracking-wide text-grey-darker text-xs font-light mb-1"
                                                   for="grid-password">
                                                Produit
                                            </label>
                                            <input class="appearance-none block w-full bg-grey-200 text-grey-darker border border-grey-200 rounded py-3 px-4 mb-3 leading-tight focus:outline-none focus:bg-white focus:border-grey"
                                            name="product_name" id="product_name" type="text" >
                                            
                                        </div>
                                    </div>
                                    <div class="flex flex-wrap -mx-3 mb-2">
                                        <div class="w-full md:w-1/3 px-3 mb-6 md:mb-0">
                                            <label class="block uppercase tracking-wide text-grey-darker text-xs font-light mb-1"
                                                   for="grid-city">
                                                Quantité
                                            </label>
                                            <input class="appearance-none block w-full bg-grey-200 text-grey-darker border border-grey-200 rounded py-3 px-4 leading-tight focus:outline-none focus:bg-white focus:border-grey"
                                            type="number" name="quantity" id="quantity" >
                                        </div>
                                        <div class="w-full md:w-1/3 px-3 mb-6 md:mb-0">
                                            <label class="block uppercase tracking-wide text-grey-darker text-xs font-light mb-1"
                                                   for="grid-state">
                                                Prix
                                            </label>
                                            <div class="relative">
                                            <input class="appearance-none block w-full bg-grey-200 text-grey-darker border border-grey-200 rounded py-3 px-4 leading-tight focus:outline-none focus:bg-white focus:border-grey"
                                            type="number" name="price" id="price" >
                                                
                                            </div>
                                        </div>
                                        <button class="bg-transparent hover:bg-blue-500 text-blue-dark font-semibold hover:text-white py-2 px-4 border border-blue hover:border-transparent rounded" type="submit">Ajouter la commande</button>
                                    </div>
                                </form>
                                
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
<script src="form_validation.js"></script>

</body>

</html>