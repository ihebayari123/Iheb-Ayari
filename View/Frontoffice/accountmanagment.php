<!DOCTYPE HTML>
<html>
	<head>
		<title>Account - AgriAura</title>
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
					<span class="icon fa-user-circle"></span>
				</div>
				<div class="content">
					<div class="inner">
						<h1>Account Management</h1>
						<p>Manage your account details and access.</p>
					</div>
				</div>
				<nav>
					<ul>
						<li><a href="index.php">Home</a></li>
						<li><a href="account.php" class="active">signup</a></li>
						<li><a href="signin.php">Sign In</a></li>
						<li><a href="profiluser.php">Profile</a></li>
					</ul>
				</nav>
			</header>
               
            
			<!-- Main -->
			<div id="main">
				<!-- Account Form -->
				<article id="account">
					<h2 class="major">Create Account</h2>
					<form method="POST" action="http://localhost/user2/View/Backoffice/adduser.php">
						<div class="fields">
							<div class="field">
								<label for="Name">Name</label>
								<input type="text" id="Name" name="Name" required />
							</div>
							<div class="field">
								<label for="surname">Surname</label>
								<input type="text" id="surname" name="surname" required />
							</div>
							<div class="field">
								<label for="Email">Email</label>
								<input type="email" id="Email" name="Email" required />
							</div>
							<div class="field">
								<label for="password">Password</label>
								<input type="password" id="password" name="password" required />
							</div>
							<div class="field">
								<label for="role">Role</label>
								<select id="role" name="role" required>
									<option value="User">User</option>
									<option value="Admin">Admin</option>
								</select>
							</div>
						</div>
						<ul class="actions">
							<li><button type="submit" class="button primary">Create Account</button></li>
							<li><a href="signin.php" class="button">Sign In</a></li>
						</ul>
					</form>
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
