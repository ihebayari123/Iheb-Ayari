<?php
include '../../Controller/userController.php';

$userC = new userController();

if (isset($_GET['id'])) {
    $id = $_GET['id'];
    $user = $userC->getuser($id); // Récupérer les données actuelles de l'utilisateur
}

if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    $name = $_POST['Name'];
    $surname = $_POST['surname'];
    $email = $_POST['Email'];
    $password = $_POST['password'];
    $role = $_POST['role']; // Récupérer le rôle

    // Mettre à jour l'utilisateur dans la base de données
    $userC->updateuser($id, $name, $surname, $email, $password, $role);

    // Redirection vers la liste des utilisateurs
    header("Location: userList.php");
    exit();
}
?>

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Update User</title>
    <!-- Styles -->
    <link rel="stylesheet" href="./dist/styles.css">
    <link rel="stylesheet" href="./dist/all.css">
    <link href="https://fonts.googleapis.com/css?family=Source+Sans+Pro:400,400i,600,600i,700,700i" rel="stylesheet">
    <script src="./controleDeSaisie.js"></script>
</head>

<body>
    <div class="mx-auto bg-grey-lightest">
        <div class="min-h-screen flex flex-col">
            <header class="bg-nav">
                <div class="flex justify-between">
                    <div class="p-1 mx-3 inline-flex items-center">
                        <h1 class="text-white p-2">Update User</h1>
                    </div>
                </div>
            </header>

            <div class="flex flex-1">
                <aside id="sidebar" class="bg-side-nav w-1/2 md:w-1/6 lg:w-1/6 border-r border-side-nav hidden md:block lg:block">
                    <ul class="list-reset flex flex-col">
                        <li class="w-full h-full py-3 px-2 border-b border-light-border">
                            <a href="index.html" class="text-nav-item no-underline">
                                Dashboard
                            </a>
                        </li>
                        <li class="w-full h-full py-3 px-2 border-b border-light-border bg-white">
                            <a href="userList.php" class="text-nav-item no-underline">
                                User List
                            </a>
                        </li>
                    </ul>
                </aside>

                <main class="bg-white-300 flex-1 p-3 overflow-hidden">
                    <div class="flex flex-col">
                        <div class="mb-2 border-solid border-gray-300 rounded border shadow-sm w-full">
                            <div class="bg-gray-200 px-2 py-3 border-b">
                                Update User Details
                            </div>
                            <div class="p-3">
                                <form action="" method="POST" class="w-full">
                                    <div class="mb-4">
                                        <label for="Name" class="block text-sm font-bold mb-2">Name</label>
                                        <input type="text" name="Name" class="shadow border rounded w-full py-2 px-3" value="<?php echo htmlspecialchars($user['Name']); ?>" required>
                                    </div>

                                    <div class="mb-4">
                                        <label for="surname" class="block text-sm font-bold mb-2">Surname</label>
                                        <input type="text" name="surname" class="shadow border rounded w-full py-2 px-3" value="<?php echo htmlspecialchars($user['surname']); ?>" required>
                                    </div>

                                    <div class="mb-4">
                                        <label for="Email" class="block text-sm font-bold mb-2">Email</label>
                                        <input type="email" name="Email" class="shadow border rounded w-full py-2 px-3" value="<?php echo htmlspecialchars($user['Email']); ?>" required>
                                    </div>

                                    <div class="mb-4">
                                        <label for="password" class="block text-sm font-bold mb-2">Password</label>
                                        <input type="password" name="password" class="shadow border rounded w-full py-2 px-3" value="<?php echo htmlspecialchars($user['password']); ?>" required>
                                    </div>

                                    <div class="mb-4">
                                        <label for="role" class="block text-sm font-bold mb-2">Role</label>
                                        <select name="role" class="shadow border rounded w-full py-2 px-3" required>
                                            <option value="Admin" <?php echo ($user['role'] === 'Admin' ? 'selected' : ''); ?>>Admin</option>
                                            <option value="User" <?php echo ($user['role'] === 'User' ? 'selected' : ''); ?>>User</option>
                                        </select>
                                    </div>

                                    <div class="flex items-center justify-between">
                                        <button type="submit" class="bg-teal-500 hover:bg-teal-700 text-white font-bold py-2 px-4 rounded">
                                            Update User
                                        </button>
                                        <a href="userList.php" class="text-teal-500 hover:text-teal-800">
                                            Cancel
                                        </a>
                                    </div>
                                </form>
                            </div>
                        </div>
                    </div>
                </main>
            </div>
        </div>
    </div>
</body>

</html>
