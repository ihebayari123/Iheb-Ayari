<?php
// Database connection
$host = 'localhost'; // Change this as needed
$db = 'agriaura';
$user = 'root';
$pass = '';
$dsn = "mysql:host=$host;dbname=$db;charset=utf8mb4";

try {
    $pdo = new PDO($dsn, $user, $pass, [
        PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
        PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
    ]);
} catch (PDOException $e) {
    die("Database connection failed: " . $e->getMessage());
}

// Fetch products
$search = $_GET['search'] ?? '';
$category = $_GET['category'] ?? '';
$page = $_GET['page'] ?? 1;
$itemsPerPage = 6;
$offset = ($page - 1) * $itemsPerPage;

// Start building the SQL query
$sql = "SELECT * FROM produits WHERE name LIKE :search";
$params = [':search' => "%$search%"];

if (!empty($category)) {
    $sql .= " AND id = :category";
    $params[':category'] = $category;
}

// Manually insert the limit and offset values into the query string
$sql .= " LIMIT $itemsPerPage OFFSET $offset";

// Prepare and execute the query
$stmt = $pdo->prepare($sql);
$stmt->execute($params);

// Fetch products
$products = $stmt->fetchAll();

// Fetch categories for the dropdown
$categoryStmt = $pdo->query("SELECT * FROM categories");
$categories = $categoryStmt->fetchAll();

// Get total product count for pagination
$countStmt = $pdo->prepare("SELECT COUNT(*) FROM produits WHERE name LIKE :search");
$countStmt->execute([':search' => "%$search%"]);
$totalItems = $countStmt->fetchColumn();
$totalPages = ceil($totalItems / $itemsPerPage);
?> 

<?php
// Assuming product images are stored in a folder with filenames like product-1.jpg, product-2.jpg, etc.
$productImageFolder = 'C:/xampp/htdocs/ProjetWeb2A/View/FrontOffice/assets/img/products/';
$images = [];

// Use glob to get all files in the directory that match the pattern
foreach (glob($productImageFolder . 'product-*.jpg') as $image) {
    // Add image path to the images array
    $images[] = $image;
}

// Sort images by filename (the product number)
usort($images, function($a, $b) {
    // Extract the numbers from filenames like 'product-1.jpg' => 1, 'product-2.jpg' => 2
    preg_match('/product-(\d+)\.jpg/', $a, $matchesA);
    preg_match('/product-(\d+)\.jpg/', $b, $matchesB);
    
    // Compare the extracted numbers for sorting
    return (int)$matchesA[1] - (int)$matchesB[1];
});
?>

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="utf-8">
    <meta content="width=device-width, initial-scale=1.0" name="viewport">
    <title>Products - AgriSmart</title>

    <!-- Header -->
    <header id="header" class="header d-flex align-items-center position-relative">
        <a href="index.html" class="logo d-flex align-items-center">
            <!-- Logo here -->
        </a>
    </header>

    <!-- Favicons -->
    <link href="assets/img/favicon.png" rel="icon">
    <link href="assets/img/apple-touch-icon.png" rel="apple-touch-icon">

    <!-- Fonts -->
    <link href="https://fonts.googleapis.com" rel="preconnect">
    <link href="https://fonts.gstatic.com" rel="preconnect" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Open+Sans:wght@300;700&display=swap" rel="stylesheet">

    <!-- Vendor CSS Files -->
    <link href="assets/vendor/bootstrap/css/bootstrap.min.css" rel="stylesheet">
    <link href="assets/vendor/bootstrap-icons/bootstrap-icons.css" rel="stylesheet">

    <!-- Main CSS File -->
    <link href="assets/css/main.css" rel="stylesheet">

    <style>
        /* Custom background for the whole page */
        body {
            background-color: #f0f9f0; /* Light green background to create an organic feel */
            background-image: url('assets/img/agriculture-bg.jpg'); /* Set a background image related to agriculture */
            background-size: cover;
            background-position: center;
            color: #333;
        }

        /* Header section with farm field image */
        header {
            background: url('assets/img/agriculture-header.jpg') no-repeat center center;
            background-size: cover;
            padding: 80px 0;
            text-align: center;
            color: #fff;
        }

        header h1 {
            font-family: 'Arial', sans-serif;
            font-size: 2.5rem;
        }

        /* Product section with green, earthy tones */
        .products-section {
            background-color: #ffffff;
            padding: 40px 0;
        }

        .products-section .card {
            border: 1px solid #d8d8d8;
            background-color: #f9f9f9;
            box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1);
        }

        .products-section .card-body {
            background-color: #e9f6e3; /* Light green background */
            color: #4d4d4d;
        }

        .products-section .card-title {
            font-family: 'Verdana', sans-serif;
            font-size: 1.2rem;
        }

        .products-section .text-primary {
            color: #009900; /* Green for prices */
        }

        .products-section .btn {
            background-color: #66cc33;
            color: white;
            border: none;
        }

        .products-section .btn:hover {
            background-color: #009900;
        }

        /* Footer with earthy texture */
        footer {
            background: #2e3b2e; /* Dark green footer */
            padding: 20px 0;
            color: #fff;
            text-align: center;
        }

        footer .credits {
            color: #b3b3b3;
        }

        /* Pagination buttons with green accents */
        .blog-pagination ul li a {
            background-color: #66cc33;
            color: white;
            border-radius: 5px;
        }

        .blog-pagination ul li a:hover {
            background-color: #009900;
        }

        /* Style for search input and category dropdown */
        form .form-control, .form-select {
            border-radius: 5px;
            background-color: #e6f9e6;
            color: #4d4d4d;
            border: 1px solid #b3b3b3;
        }

        form .btn-primary {
            background-color: #66cc33;
            color: white;
            border: none;
        }

        form .btn-primary:hover {
            background-color: #009900;
        }
    </style>

</head>

<body>
    <!-- Navbar -->
    <nav class="navbar navbar-expand-lg navbar-light bg-light">
        <div class="container-fluid">
            <a class="navbar-brand" href="index.php"><img src="assets/img/logo.png" alt="AgriCulture" height="40"></a>
            <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarNav" aria-controls="navbarNav" aria-expanded="false" aria-label="Toggle navigation">
                <span class="navbar-toggler-icon"></span>
            </button>
            <div class="collapse navbar-collapse" id="navbarNav">
                <ul class="navbar-nav ms-auto">
                    <li class="nav-item"><a class="nav-link" href="index.php">Home</a></li>
                    <li class="nav-item"><a class="nav-link" href="about.php">About Us</a></li>
                    <li class="nav-item"><a class="nav-link" href="services.php">Panier</a></li>
                    <li class="nav-item"><a class="nav-link active" href="products.php">Products</a></li>
                    <li class="nav-item"><a class="nav-link" href="contact.php">Réclamation</a></li>
                </ul>
            </div>
        </div>
    </nav>

    <main>
        <!-- Page Title -->
        <div class="page-title dark-background" data-aos="fade" style="background-image: url(assets/img/bg.jpg);">
            <div class="container position-relative">
                <h1>Products</h1>
                <p>Home / Products</p>
                <nav class="breadcrumbs">
                    <ol>
                        <li><a href="index.html">Home</a></li>
                        <li class="current">Products</li>
                    </ol>
                </nav>
            </div>
        </div><!-- End Page Title -->

        <!-- Search & Filter -->
        <div class="container mt-3">
            <form method="GET" action="View/Backoffice/produitList.php">
                <div class="row">
                    <div class="col-md-4">
                        <input type="text" name="search" class="form-control" placeholder="Search..." value="<?= htmlspecialchars($search) ?>">
                    </div>
                    <div class="col-md-4">
                        <select name="category" class="form-select">
                            <option value="">All Categories</option>
                            <?php foreach ($categories as $cat): ?>
                                <option value="<?= $cat['id'] ?>" <?= $category == $cat['id'] ? 'selected' : '' ?>><?= htmlspecialchars($cat['name']) ?></option>
                            <?php endforeach; ?>
                        </select>
                    </div>
                    <div class="col-md-4">
                        <button type="submit" class="btn btn-primary">Search</button>
                    </div>
                </div>
            </form>
        </div>
                    
        <!-- Products -->
        <div class="products-section">
            <div class="container">
                <div class="row">
                    <?php foreach ($products as $index => $product): ?>
                        <div class="col-lg-4 col-md-6 mb-4">
                            <div class="card">
                                <img src="<?= 'assets/img/products/' . basename($images[$index]) ?>" class="card-img-top" alt="<?= htmlspecialchars($product['name']) ?>">
                                <div class="card-body">
                                    <h5 class="card-title"><?= htmlspecialchars($product['name']) ?></h5>
                                    <p class="card-text"><?= htmlspecialchars($product['description']) ?></p>
                                    <span class="text-primary"><?= number_format($product['price'], 2) ?> TND</span>
                                    <a href="productDetails1.php?id=<?= $product['id'] ?>" class="btn">commander</a>
                                </div>
                            </div>
                        </div>
                    <?php endforeach; ?>
                </div>

                <!-- Pagination -->
                <div class="blog-pagination text-center">
                    <ul class="pagination">
                        <li class="page-item <?= $page <= 1 ? 'disabled' : '' ?>"><a class="page-link" href="?page=<?= $page - 1 ?>&search=<?= $search ?>&category=<?= $category ?>">Previous</a></li>
                        <li class="page-item <?= $page >= $totalPages ? 'disabled' : '' ?>"><a class="page-link" href="?page=<?= $page + 1 ?>&search=<?= $search ?>&category=<?= $category ?>">Next</a></li>
                    </ul>
                </div>
            </div>
        </div>

    </main>

    <!-- Footer -->
    <footer>
        <p>&copy; 2024 AgriSmart - All Rights Reserved</p>
        <p class="credits">Designed by AgriTech Solutions</p>
    </footer>

    <!-- Vendor JS Files -->
    <script src="assets/vendor/bootstrap/js/bootstrap.bundle.min.js"></script>

</body>

</html>
