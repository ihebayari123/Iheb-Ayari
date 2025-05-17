<?php
include '../../Controller/reclamationController.php';

// Default sorting column and order
$sortColumn = isset($_GET['sortColumn']) ? $_GET['sortColumn'] : 'id';
$sortOrder = isset($_GET['sortOrder']) ? $_GET['sortOrder'] : 'ASC';

// Secure the sorting inputs
$validColumns = ['id', 'nom'];
if (!in_array($sortColumn, $validColumns)) {
    $sortColumn = 'id';
}
$sortOrder = ($sortOrder === 'DESC') ? 'DESC' : 'ASC';

// Instantiate the controller and fetch data
$gestionreclamationC = new reclamationController();
$list = $gestionreclamationC->listreclamation($sortColumn, $sortOrder);
$resolutionList = $gestionreclamationC->listResolutionReclamations(); // Fetch resolutions
// Vérifier si une recherche est effectuée
$search = isset($_GET['search']) ? $_GET['search'] : null;

// Si une recherche est présente, chercher par nom
if ($search) {
    $list = $gestionreclamationC->searchReclamationByName($search);
} else {
    $list = $gestionreclamationC->listreclamation($sortColumn, $sortOrder);
}

?>

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <!-- CSS -->
    <link rel="stylesheet" href="./dist/styles.css">
    <link rel="stylesheet" href="./dist/all.css">
    <link href="https://fonts.googleapis.com/css?family=Source+Sans+Pro:400,400i,600,600i,700,700i" rel="stylesheet">
    <title>Tables | Reclamation List</title>
    

    <!-- Custom CSS  for rechrech et tri-->
    <style>
        /* General Form Styling */
        form {
            background-color: #fff;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1);
            margin-bottom: 30px;
            display: flex;
            flex-wrap: wrap;
            gap: 10px;
            justify-content: space-between;
        }

        label {
            font-weight: 600;
            color: #4a5568;
            margin-right: 10px;
        }

        /* Dropdown Styling */
        select, input[type="text"] {
            padding: 10px;
            border-radius: 5px;
            border: 1px solid #e2e8f0;
            font-size: 14px;
            color: #4a5568;
            outline: none;
            transition: border-color 0.3s ease;
        }

        select:focus, input[type="text"]:focus {
            border-color: #3182ce;
        }

        /* Button Styling */
        button, input[type="submit"] {
            background-color: #4299e1;
            color: white;
            padding: 10px 20px;
            border-radius: 5px;
            border: none;
            font-size: 14px;
            cursor: pointer;
            transition: background-color 0.3s ease;
        }

        button:hover, input[type="submit"]:hover {
            background-color: #3182ce;
        }

        .bg-red-500 {
            background-color: #f56565;
        }
        
        .bg-blue-500 {
            background-color: #4299e1;
        }

        .bg-blue-500:hover {
            background-color: #3182ce;
        }

        .bg-red-500:hover {
            background-color: #e53e3e;
        }

        /* Aligning the forms and buttons */
        .form-container {
            display: flex;
            justify-content: space-between;
            gap: 20px;
            width: 100%;
        }

        .form-container > div {
            flex: 1;
        }

        .form-container input[type="text"], .form-container select {
            width: 100%;
        }
    </style>

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
                    <h1 class="text-white p-2">Reclamations</h1>
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
                        </li>
                        <li class="py-3 px-2 border-b">
                            <a href="forms.php" class="text-nav-item">orders</a>
                        </li>
                </ul>
            </aside>
            <!--/Sidebar-->

            <!--Main Content-->
            <main class="bg-white-500 flex-1 p-3 overflow-hidden">
                <div class="flex flex-col">
                    
                    
                   
    <!-- Forms -->
    <form method="GET" action="">
        <div class="form-container">
            <div>
                <label for="sortColumn">Trier par:</label>
                <select name="sortColumn" id="sortColumn">
                    <option value="id">ID</option>
                    <option value="nom">Nom du Client</option>
                </select>
            </div>
            <div>
                <label for="sortOrder">Ordre:</label>
                <select name="sortOrder" id="sortOrder">
                    <option value="ASC">Croissant</option>
                    <option value="DESC">Décroissant</option>
                </select>
            </div>
            <button type="submit" class="bg-red-500 text-white py-1 px-3 rounded text-xs ml-2">Trier</button>
        </div>
    </form>

    <form method="GET" action="">
        <div class="form-container">
            <div>
                <input type="text" name="search" placeholder="Rechercher un client" class="border py-1 px-3 rounded text-sm" />
            </div>
            <button type="submit" class="bg-blue-500 text-white py-1 px-3 rounded text-xs ml-2">Rechercher</button>
        </div>
    </form>


   

</form>





<div class="mb-4">
                        <h1 class="text-xl font-semibold">Liste des Réclamations</h1>
                    </div>
                    <div class="bg-white shadow-md rounded my-6">
                        <table class="min-w-max w-full table-auto">
                            <thead>
                                <tr class="bg-gray-200 text-gray-600 uppercase text-sm leading-normal">
                                    <th class="py-3 px-6 text-left">ID</th>
                                    <th class="py-3 px-6 text-left">Nom</th>
                                    <th class="py-3 px-6 text-left">Produit</th>
                                    <th class="py-3 px-6 text-left">Description</th>
                                    <th class="py-3 px-6 text-left">Date</th>
                                    <th class="py-3 px-6 text-left">Statut</th>
                                    <th class="py-3 px-6 text-center">Actions</th>
                                </tr>
                            </thead>
                            <tbody class="text-gray-600 text-sm font-light">
                                <?php foreach ($list as $reclamation): ?>
                                    <tr class="border-b border-gray-200 hover:bg-gray-100">
                                        <td class="py-3 px-6 text-left"><?php echo $reclamation['id']; ?></td>
                                        <td class="py-3 px-6 text-left"><?php echo $reclamation['nom']; ?></td>
                                        <td class="py-3 px-6 text-left"><?php echo $reclamation['produit']; ?></td>
                                        <td class="py-3 px-6 text-left"><?php echo $reclamation['description']; ?></td>
                                        <td class="py-3 px-6 text-left"><?php echo $reclamation['date']; ?></td>
                                        <td class="py-3 px-6 text-left"><?php echo $reclamation['statut']; ?></td>
                                        <td class="py-3 px-6 text-center">
                                            <?php if ($reclamation['statut'] != 'Résolu'): ?>
                                                <a href="resolvereclamation.php?id=<?php echo $reclamation['id']; ?>" 
                                                   class="bg-green-500 text-white py-1 px-3 rounded text-xs">
                                                    Résoudre
                                                </a>
                                            <?php else: ?>
                                                <span class="bg-gray-400 text-white py-1 px-3 rounded text-xs">Résolu</span>
                                            <?php endif; ?>
                                            <a href="deletereclamation.php?id=<?php echo $reclamation['id']; ?>" 
                                               class="bg-red-500 text-white py-1 px-3 rounded text-xs ml-2">
                                                Supprimer
                                            </a>
                                            <a href="updatereclamation.php?id=<?php echo $reclamation['id']; ?>" 
                                               class="bg-blue-500 text-white py-1 px-3 rounded text-xs ml-2">
                                                Modifier
                                            </a>
                                        </td>
                                    </tr>
                                <?php endforeach; ?>
                            </tbody>
                        </table>
                    </div>

                    <!-- Résolutions des réclamations -->
<div class="bg-white shadow-md rounded my-6">
    <h2 class="text-xl font-semibold">Liste des Résolutions</h2>
    <table class="min-w-max w-full table-auto">
        <thead>
            <tr class="bg-gray-200 text-gray-600 uppercase text-sm leading-normal">
                <th class="py-3 px-6 text-left">ID</th>
                <th class="py-3 px-6 text-left">Reclamation ID</th>
                <th class="py-3 px-6 text-left">Statut</th>
                
                <th class="py-3 px-6 text-left">Assigned To</th>
                <th class="py-3 px-6 text-left">Description</th>
                <th class="py-3 px-6 text-left">Méthode</th>
                <th class="py-3 px-6 text-left">Date Suivi</th>
                <th class="py-3 px-6 text-center">Actions</th> <!-- Nouvelle colonne Actions -->
            </tr>
        </thead>
        <tbody class="text-gray-600 text-sm font-light">
            <?php foreach ($resolutionList as $resolution): ?>
                <tr class="border-b border-gray-200 hover:bg-gray-100">
                    <td class="py-3 px-6 text-left"><?php echo $resolution['id']; ?></td>
                    <td class="py-3 px-6 text-left"><?php echo $resolution['reclamation_id']; ?></td>
                    <td class="py-3 px-6 text-left"><?php echo $resolution['statut']; ?></td>
                   
                    <td class="py-3 px-6 text-left"><?php echo $resolution['assigned_to']; ?></td>
                    <td class="py-3 px-6 text-left"><?php echo $resolution['resolution_description']; ?></td>
                    <td class="py-3 px-6 text-left"><?php echo $resolution['resolution_method']; ?></td>
                    <td class="py-3 px-6 text-left"><?php echo $resolution['follow_up_date']; ?></td>
                    <td class="py-3 px-6 text-center">
                        <a href="deleteresolution.php?id=<?php echo $resolution['id']; ?>" 
                           class="bg-red-500 text-white py-1 px-3 rounded text-xs ml-2">
                            Supprimer
                        </a>
                        <a href="updateresolution.php?id=<?php echo $resolution['id']; ?>" 
                           class="bg-blue-500 text-white py-1 px-3 rounded text-xs ml-2">
                            Modifier
                        </a>
                        <form action="validation.php" method="POST">
    <input type="hidden" name="reclamation_id" value="<?php echo $reclamation['id']; ?>" />
    <input type="submit" value="Valider" name="valider_resolution" class="bg-green-500 text-white py-2 px-4 rounded shadow-md hover:bg-green-600 transition-all" />
</form>
                    </td>
                </tr>
            <?php endforeach; ?>
        </tbody>
    </table>
</div>


            </main>
            <!--/Main Content-->
        </div>

        <!--Footer-->
        <footer class="bg-grey-darkest text-white p-2">
            <div class="flex flex-1 mx-auto">&copy; My Design</div>
        </footer>
        <!--/Footer-->
    </div>
</div>
<script src="./main.js"></script>
</body>
</html>
