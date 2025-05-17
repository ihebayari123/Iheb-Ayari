
<?php
ob_start(); // Enable output buffering
session_start();
$nom = $_SESSION['user_name'] ?? null;
?>


<!DOCTYPE HTML>
<html>
	<head>
		<title>FORUM</title>
		<meta charset="utf-8" />
		<meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no" />
		<link rel="stylesheet" href="assets/css/main.css" />
		<link rel="stylesheet" href="assets/css/style3.css" />
		<link rel="stylesheet" href="style2.css" />
		<noscript><link rel="stylesheet" href="assets/css/noscript.css" /></noscript>
        <style>
			.account-name {
    position: fixed; /* Keep it fixed to the top-left */
    top: 10px;
    left: 10px;
    font-size: 16px;
    font-weight: bold;
    max-width: 150px; /* Allow it to shrink or expand */
    text-align: left;
    z-index: 100;
}

.account-name a {
    text-decoration: none;
    color: #000; /* Adjust color as needed */
}

.account-name span {
    color: white; /* Adjust color for logged-in name */
    word-break: break-word; /* Prevent overflow */
}

		</style>
		
	</head>
	
	<body class="is-preload">
     
	
		<!-- Wrapper -->
			<div id="wrapper">
			<!-- Account section -->
			<div class="header-container">
    <div class="account-name">
        <?php if (isset($_SESSION['user_name'])): ?>
            <span>Welcome, <?php echo htmlspecialchars($nom); ?>!</span>
        <?php else: ?>
            <a href="signin.php">Log in</a>
        <?php endif; ?>
    </div>
</div>


				<!-- Header -->
				<div class="logo-circle"> 
					
				</div>
					<header id="header">
						<div class="logo">
						
            <img id="log" src="images/logo backremo.png" alt="Agriaura Logo" width="80" height="80" />
       

						</div>
						<div class="content">
							<div class="inner">
								<h1>agriaura</h1>
								<p>check our website
								</p>
							</div>
						</div>
						<nav>
							<ul>
								<li><a href="#intro">Forum</a></li>
								<li><a href="blog.php">Products</a></li>
								<li><a href="accountmanagment.php" class="active">account</a></li>
								<li><a href="addgestionreclamation.php" class="active">reclamation</a></li>
								<li><a href="index.html">About Us</a></li>
								
							</ul>
						</nav>
						
					</header>
                          
				<!-- Main -->
					<div id="main">

						<!-- Intro -->
							<article id="intro">
								<h2 class="major">Posts</h2>
								<span class="image main"><img src="images/pic01.webp" alt="" /></span>
								
								<div class="search-panels">
									<div class="search-group">
										
										<input required="" type="text" name="text" autocomplete="on" class="inputsearch">
										<label class="enter-label">Search</label>
										<div class="btn-box">
											<button class="btn-search">
												<svg xmlns="http://www.w3.org/2000/svg" height="1em" viewBox="0 0 512 512"><path d="M416 208c0 45.9-14.9 88.3-40 122.7L502.6 457.4c12.5 12.5 12.5 32.8 0 45.3s-32.8 12.5-45.3 0L330.7 376c-34.4 25.2-76.8 40-122.7 40C93.1 416 0 322.9 0 208S93.1 0 208 0S416 93.1 416 208zM208 352a144 144 0 1 0 0-288 144 144 0 1 0 0 288z"></path><circle id="svg-circle" cx="208" cy="208" r="144"></circle></svg>
											</button>
										</div>
										<div class="btn-box-x">
											<button class="btn-cleare">
											<svg xmlns="http://www.w3.org/2000/svg" height="1em" viewBox="0 0 384 512"><path d="M342.6 150.6c12.5-12.5 12.5-32.8 0-45.3s-32.8-12.5-45.3 0L192 210.7 86.6 105.4c-12.5-12.5-32.8-12.5-45.3 0s-12.5 32.8 0 45.3L146.7 256 41.4 361.4c-12.5 12.5-12.5 32.8 0 45.3s32.8 12.5 45.3 0L192 301.3 297.4 406.6c12.5 12.5 32.8 12.5 45.3 0s12.5-32.8 0-45.3L237.3 256 342.6 150.6z" id="cleare-line"></path></svg>
											</button>
										</div>	
									  </form>	
									</div>
								</div>		
								
    <button class="button-add">Add Post</button>


                                <!-- Form container -->
								<div id="discussion-form-container" class="add-container">
                      			  <form action="addForum.php" method="POST" onsubmit="return validateForm()">
                           			 <input type="text" id="title" name="title" class="input" placeholder="Title">
										<input type="text" id="author" name="author" class="input" placeholder="Author" value="<?php echo isset($_SESSION['user_name']) ? htmlspecialchars($_SESSION['user_name']) : ''; ?>" readonly>

                           			 <textarea id="content" name="content" class="input" placeholder="Content"></textarea>
                           			 <button class="button-create">create new discussion</button>
                      			  </form>
                    			</div>
                                <div id="forum-list">
   									 <?php include 'ForumList2.php'; ?>
								</div>

                <div id="update-form-container"></div>
                            </article>

						<!-- Work -->
							<article id="work">
								<h2 class="major">Work</h2>
								<span class="image main"><img src="images/pic01.webp" alt="" /></span>
								<p>Adipiscing magna sed dolor elit. Praesent eleifend dignissim arcu, at eleifend sapien imperdiet ac. Aliquam erat volutpat. Praesent urna nisi, fringila lorem et vehicula lacinia quam. Integer sollicitudin mauris nec lorem luctus ultrices.</p>
								<p>Nullam et orci eu lorem consequat tincidunt vivamus et sagittis libero. Mauris aliquet magna magna sed nunc rhoncus pharetra. Pellentesque condimentum sem. In efficitur ligula tate urna. Maecenas laoreet massa vel lacinia pellentesque lorem ipsum dolor. Nullam et orci eu lorem consequat tincidunt. Vivamus et sagittis libero. Mauris aliquet magna magna sed nunc rhoncus amet feugiat tempus.</p>
							</article>
						
				

					</div>

				<!-- Footer -->
					<footer id="footer">
						<p class="copyright">&copy; by NextGen Creators .</p>
					</footer>
					
             

			</div>

		<!-- BG -->
		
			<div id="bg"></div>
			
			
       <!-- Include the chatbot -->
	   <?php include 'bot.php'; ?>
		<!-- Scripts -->
			<script src="assets/js/jquery.min.js"></script>
			<script src="assets/js/browser.min.js"></script>
			<script src="assets/js/breakpoints.min.js"></script>
			<script src="assets/js/util.js"></script>
			<script src="assets/js/main.js"></script>
			<script src="work.js"></script>
            <script>
				
				document.addEventListener('DOMContentLoaded', function() {
    const addButton = document.querySelector('.button-add');
    const formContainer = document.getElementById('discussion-form-container');

    addButton.addEventListener('click', function() {
        // Toggle the form visibility
        if (formContainer.style.display === "none" || formContainer.style.display === "") {
            formContainer.style.display = "flex";  // Show the form with flexbox
        } else {
            formContainer.style.display = "none";  // Hide the form
        }
    });
});

			</script>
      
	  <script>
  document.addEventListener('DOMContentLoaded', function() {
    const searchInput = document.querySelector('.inputsearch');
    const postListContainer = document.getElementById('forum-list');
    
    searchInput.addEventListener('input', function() {
      const searchText = searchInput.value;

      
      const xhr = new XMLHttpRequest();
      xhr.open('GET', 'ForumList2.php?text=' + encodeURIComponent(searchText), true);
      
      xhr.onload = function() {
        if (xhr.status === 200) {
         
          postListContainer.innerHTML = xhr.responseText;
        }
      };

      xhr.send();
    });

    // Toggle the visibility of the add post form
    const addButton = document.querySelector('.button-add');
    const formContainer = document.getElementById('discussion-form-container');

    addButton.addEventListener('click', function() {
      if (formContainer.style.display === "none" || formContainer.style.display === "") {
        formContainer.style.display = "block";
      } else {
        formContainer.style.display = "none";
      }
    });
  });

  
</script>



			
	</body>
</html>
