<?php
require_once '../../controller/controller.php';  // Inclure le contrôleur
require_once '../../config.php';  // Inclure la connexion à la base de données

// Récupérer l'instance PDO
$conn = config::getConnexion();

$orderController = new OrderController($conn);

// Récupérer les IDs des commandes existantes avec leur statut
$stmt = $conn->prepare("SELECT id, status FROM orders");
$stmt->execute();
$orderIds = $stmt->fetchAll(PDO::FETCH_ASSOC);

// Traitement du formulaire lors de la soumission
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $orderId = $_POST['order_id'];
    $deliveryStatus = $_POST['delivery_status'];
    $deliveryDate = $_POST['delivery_date'];

    // Valider si la commande peut recevoir une livraison
    if (!$orderController->isValidOrderForDelivery($orderId)) {
        echo "Erreur : Cette commande est déjà livrée ou annulée et ne peut pas être livrée à nouveau.";
    } else {
        // Ajouter la livraison via le contrôleur
        if ($orderController->addDelivery($orderId, $deliveryStatus, $deliveryDate)) {
            echo "Livraison ajoutée avec succès !";
        } else {
            echo "Erreur lors de l'ajout de la livraison.";
        }
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
                                <form class="w-full" action="formsdeliveries.php" method="POST" onsubmit="return validateOrderForm();">
                                    <div class="flex flex-wrap -mx-3 mb-6">
                                        <div class="w-full md:w-1/2 px-3 mb-6 md:mb-0">
                                            <label class="block uppercase tracking-wide text-gray-700 text-xs font-light mb-1"
                                                   for="grid-first-name">
                                                   ID de commande :
                                            </label>
                                            <select name="order_id" id="order_id" required>
        <option value="">-- Sélectionnez un ID --</option>
        <?php foreach ($orderIds as $order) : ?>
            <option value="<?= htmlspecialchars($order['id']) ?>">
                <?= htmlspecialchars($order['id']) ?> (Statut : <?= htmlspecialchars($order['status']) ?>)
            </option>
        <?php endforeach; ?>
    </select>
                                            
                                        </div>
                                        <div class="w-full md:w-1/2 px-3">
                                            <label class="block uppercase tracking-wide text-gray-700 text-xs font-light mb-1"
                                                   for="grid-last-name">
                                                   Statut de livraison :
                                            </label>
                                            <input class="appearance-none block w-full bg-gray-200 text-grey-darker border border-gray-200 rounded py-3 px-4 leading-tight focus:outline-none focus:bg-white-500 focus:border-gray-600"
                                            type="text" name="delivery_status" id="delivery_status" placeholder="ex : Pending">
                                        </div>
                                    </div>
                                    <div class="flex flex-wrap -mx-3 mb-6">
                                        <div class="w-full px-3">
                                            <label class="block uppercase tracking-wide text-grey-darker text-xs font-light mb-1"
                                                   for="grid-password">
                                                Date de livraison
                                            </label>
                                            <input class="appearance-none block w-full bg-grey-200 text-grey-darker border border-grey-200 rounded py-3 px-4 mb-3 leading-tight focus:outline-none focus:bg-white focus:border-grey"
                                            type="date" name="delivery_date" id="delivery_date" >
                                            
                                        </div>
                                    </div>
                                    <button class="bg-transparent hover:bg-blue-500 text-blue-dark font-semibold hover:text-white py-2 px-4 border border-blue hover:border-transparent rounded" type="submit">Ajouter la livraison</button>
                                    
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