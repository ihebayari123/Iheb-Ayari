<?php
session_start();
require_once '../../Controller/UserController.php';

// Vérifier si l'utilisateur est connecté
if (!isset($_SESSION['user_id'])) {
    header("Location: signin.php");
    exit();
}

$userId = $_SESSION['user_id'];
$db = config::getConnexion();
$controller = new userController($db);
$user = $controller->getuser($userId);

// Vérifier si l'utilisateur a été récupéré
if (!$user) {
    echo "Utilisateur introuvable.";
    exit();
}
?>
<!DOCTYPE HTML>
<html>
	<head>
		<title>User Profile - AgriAura</title>
		<meta charset="utf-8" />
		<meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no" />
		<link rel="stylesheet" href="assets2/css/main.css" />
		<noscript><link rel="stylesheet" href="assets2/css/noscript.css" /></noscript>
	</head>
	<body class="is-preload">

		<!-- Wrapper -->
		<div id="wrapper">

			<!-- Header -->
			<header id="header">
				<div class="logo">
					<span class="icon fa-user"></span>
				</div>
				<div class="content">
					<div class="inner">
						<h1>User Profile</h1>
						<p>View and manage your personal information and preferences.</p>
					</div>
				</div>
        
  <!-- Main Content -->
  <main class="main">
    <div class="container mt-5">
      <h1 class="text-center">Welcome, <?php echo htmlspecialchars($user['Name']); ?> !</h1>

      <div class="card shadow-sm">
        <div class="card-body">
          <h5 class="card-title">account informations</h5>
          <p class="card-text"><strong>Name :</strong> <?php echo htmlspecialchars($user['Name']); ?></p>
          <p class="card-text"><strong>Email :</strong> <?php echo htmlspecialchars($user['Email']); ?></p>
          
          <!-- Formulaire de modification de mot de passe -->
          <h5 class="card-title mt-4">change password</h5>
          <form action="../../Controller/changePassword.php" method="POST">
            <div class="mb-3">
              <label for="current_password" class="form-label">password actuel</label>
              <input type="password" class="form-control" name="current_password" required>
            </div>
            <div class="mb-3">
              <label for="new_password" class="form-label">New password</label>
              <input type="password" class="form-control" name="new_password" required>
            </div>
            <div class="mb-3">
              <label for="confirm_password" class="form-label">Confirm password</label>
              <input type="password" class="form-control" name="confirm_password" required>
            </div>
            <button type="submit" class="btn btn-primary">Change password</button>
          </form>
          
          <!-- Bouton de déconnexion -->
          <form action="../../Controller/logout.php" method="POST" class="mt-4">
            <button type="submit" class="btn btn-danger">Logout</button>
          </form>
        </div>
      </div>
    </div>
  </main>

  <!-- Footer -->
				<nav>
					<ul>
						<li><a href="index.php">Home</a></li>
					
					</ul>
				</nav>
			</header>

			<!-- Main -->
			<div id="main">
				<!-- User Profile -->
				<article id="profile">
					<h2 class="major">Your Profile</h2>
					<div class="profile-details">
						<p><strong>Name:</strong> John Doe</p>
						<p><strong>Email:</strong> john.doe@example.com</p>
						<p><strong>Role:</strong> User</p>
					</div>
					<ul class="actions">
						<li><a href="editprofile.php" class="button primary">Edit Profile</a></li>
						<li><a href="../Controller/changepassword.php" class="button">Change Password</a></li>
					</ul>
				</article>
			</div>

			<!-- Footer -->
			<footer id="footer">
				<p class="copyright">&copy; by NextGen Creators</p>
			</footer>

		</div>

		<!-- BG -->
		<div id="bg"></div>

		<!-- Scripts -->
		<script src="assets2/js/jquery.min.js"></script>
		<script src="assets2/js/browser.min.js"></script>
		<script src="assets2/js/breakpoints.min.js"></script>
		<script src="assets2/js/util.js"></script>
		<script src="assets2/js/main.js"></script>
	</body>
</html>