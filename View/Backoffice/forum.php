    <?php
    include('../../config.php');

    // Fetch posts using PDO
    try {
        $pdo = config::getConnexion();
        $stmt = $pdo->query("SELECT author, title, content, created_at, id FROM post");
        $posts = $stmt->fetchAll(PDO::FETCH_ASSOC); // Ensure data is fetched as an associative array
    } catch (Exception $e) {
        die('Error: ' . $e->getMessage());
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
        <link rel="stylesheet" href="style.css">
        <link href="https://fonts.googleapis.com/css?family=Source+Sans+Pro:400,400i,600,600i,700,700i" rel="stylesheet">
        <title>Forum | Forum Admin</title>
    </head>

    <body>
    <!-- Container -->
    <div class="mx-auto bg-grey-lightest">
        <!-- Screen -->
        <div class="min-h-screen flex flex-col">
            <!-- Header Section -->
            <header class="bg-nav">
                <div class="flex justify-between">
                    <div class="p-1 mx-3 inline-flex items-center">
                        <i class="fas fa-bars pr-2 text-white" onclick="sidebarToggle()"></i>
                        <h1 class="text-white p-2">forum</h1>
                    </div>
                    <div class="p-1 flex flex-row items-center">
                        
                       
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
            <!-- Sidebar -->
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
                        </li>
                        <li class="py-3 px-2 border-b">
                            <a href="forms.php" class="text-nav-item">orders</a>
                        </li>
                </ul>
            </aside>
                <!-- Main -->
                <main class="bg-white-500 flex-1 p-3 overflow-hidden">
                    <div class="flex flex-col">
                        <!-- Grid Form -->
                        <div class="flex flex-1 flex-col md:flex-row lg:flex-row mx-2">
                            <div class="mb-2 border-solid border-gray-300 rounded border shadow-sm w-full">
                            <div class="search-panels">
        <div class="search-group">
            <input required="" type="text" name="text" autocomplete="on" class="inputsearch">
            <label class="enter-label">Search</label>
            <div class="btn-box">
                <button class="btn-search">
                    <!-- Search icon SVG -->
                </button>
            </div>
            <div class="btn-box-x">
                <button class="btn-cleare">
                    <!-- Clear icon SVG -->
                </button>
            </div>
        </div>
    </div>
                                <div class="bg-gray-200 px-2 py-3 border-solid border-gray-200 border-b">
                                    Post List
                                </div>
                                <div id="forum-list">
                                <div class="p-3">
                                    <table class="table-responsive w-full rounded">
                                        <thead>
                                            <tr>
                                                <th class="border w-1/4 px-4 py-2">Author</th>
                                                <th class="border w-1/6 px-4 py-2">Title</th>
                                                <th class="border w-1/6 px-4 py-2">Content</th>
                                                <th class="border w-1/6 px-4 py-2">Date</th>
                                                <th class="border w-1/7 px-4 py-2">ID</th>
                                                <th class="border w-1/7 px-4 py-2">Actions</th>
                                            </tr>
                                        </thead>
                                        <tbody>
                                        <?php if ($posts): ?>
                                            <?php foreach ($posts as $post): ?>
                                                <tr>
                                                    <td class="border px-4 py-2"><?php echo htmlspecialchars($post['author']); ?></td>
                                                    <td class="border px-4 py-2"><?php echo htmlspecialchars($post['title']); ?></td>
                                                    <td class="border px-4 py-2"><?php echo htmlspecialchars($post['content']); ?></td>
                                                    <td class="border px-4 py-2"><?php echo htmlspecialchars($post['created_at']); ?></td>
                                                    <td class="border px-4 py-2"><?php echo htmlspecialchars($post['id']); ?></td>
                                                    <td class="border px-4 py-2">
                                                        <a href="deleteForum.php?id=<?php echo $post['id']; ?>" class="bg-teal-300 cursor-pointer rounded p-1 mx-1 text-red-500" onclick="return confirm('Are you sure you want to delete this post?');">
                                                            <i class="fas fa-trash"></i>
                                                        </a>
                                                    </td>
                                                </tr>
                                            <?php endforeach; ?>
                                        <?php else: ?>
                                            <tr>
                                                <td colspan="6" class="border px-4 py-2">No posts found</td>
                                            </tr>
                                        <?php endif; ?>
                                        </tbody>
                                    </table>
                                </div>
                                        </div>
                            </div>
                        </div>
                        <!--/Grid Form -->
                    </div>
                </main>
            </div>
            <!-- Footer -->
            <footer class="bg-grey-darkest text-white p-2">
                <div class="flex flex-1 mx-auto">&copy; My Design</div>
            </footer>
        </div>
    </div>

    <script src="./main.js"></script>
    <script src="workBK.js"></script>
    </body>

    </html>
