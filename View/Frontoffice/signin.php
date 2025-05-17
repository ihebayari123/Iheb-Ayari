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
						<h1>Sign In</h1>
						<p>Access your account to manage your profile and preferences.</p>
					</div>
				</div>
				  <!-- Main Content -->
  <main class="main">
    <div class="container mt-5">
      <h1 class="text-center">Se connecter</h1>

      <form action="../../Controller/login.php" method="POST" class="login-form">
        <div class="form-group mb-3">
          <label for="email">Email :</label>
          <input type="email" name="email" id="email" class="form-control" required placeholder="Entrez votre email">
        </div>

        <div class="form-group mb-3">
          <label for="password">Mot de passe :</label>
          <input type="password" name="password" id="password" class="form-control" required placeholder="Entrez votre mot de passe">
        </div>

        <button type="submit" class="btn btn-primary w-100">Se connecter</button>

        <p class="text-center mt-3">
          Vous n'avez pas de compte ? <a href="account.php">signup</a>
        </p>
		
      </form>
	  
    </div>
	
  </main>

  <!-- Footer -->
  <nav>
					<ul>
						<li><a href="index.php">Home</a></li>
					
					</ul>
				</nav>
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
