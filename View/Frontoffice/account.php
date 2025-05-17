<!DOCTYPE HTML>
<html>
	<head>
		<title>Sign Up - AgriAura</title>
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
					<span class="icon fa-user-plus"></span>
				</div>
				<div class="content">
					<div class="inner">
						<h1>Sign Up</h1>
						<p>Create your account to access all features.</p>
					</div>
				  <!-- Formulaire pour ajouter un utilisateur -->
  <main class="main">
    <div class="container">
      <form method="POST" action="http://localhost/ProjectForum/View/Backoffice/adduser.php">
        <div class="form-group">
            <label for="Name">Name</label>
            <input type="text" class="form-control" id="Name" name="Name" required>
        </div>
        <div class="form-group">
            <label for="surname">Surname</label>
            <input type="text" class="form-control" id="surname" name="surname" required>
        </div>
        <div class="form-group">
            <label for="Email">Email</label>
            <input type="email" class="form-control" id="Email" name="Email" required>
        </div>
        <div class="form-group">
            <label for="password">Password</label>
            <input type="password" class="form-control" id="password" name="password" required>
        </div>
        <div>
          <label for="role">Role</label>
          <select name="role" required>
              <option value="User" <?php if (isset($user['role']) && $user['role'] === 'User') echo 'selected'; ?>>User</option>
              <option value="Admin" <?php if (isset($user['role']) && $user['role'] === 'Admin') echo 'selected'; ?>>Admin</option>
          </select>
      </div>
      
 

        <button type="submit" class="btn btn-primary">Add User</button>
        <p class="signin">Already have an account? <a href="signin.php">Sign In</a></p>
    </form>
	
    </div>
</div>

  </main>
  <nav>
					<ul>
						<li><a href="index.php">Home</a></li>
					
					</ul>
				</nav>
  <!-- End Formulaire -->

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
        <!-- Script de contrôle de saisie -->
        <script src="controledesaisie1.js"></script>
	</body>
</html>
