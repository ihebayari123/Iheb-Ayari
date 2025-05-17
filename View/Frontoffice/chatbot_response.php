<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $userMessage = strtolower(trim($_POST['message']));
    $response = "Sorry, I didn't understand that.";

    // Predefined responses
    $responses = [
        "hey" =>"hey, how can i help you.",
        "hello" =>"hello, how can i help you.",
        "what is agriaura" => "Agriaura is a platform dedicated to agriculture, offering forums, products, and resources for agricultural enthusiasts.",
        "how can i add a forum post" => "To add a forum post, click the 'Add Post' button on the forum page and fill in the details.",
        "what products does agriaura offer" => "Agriaura offers a variety of agricultural products, including seeds, tools, and fertilizers.",
        "how do i search for posts" => "Use the search bar on the forum page to find posts by title or keywords.",
        "who created agriaura" => "Agriaura was developed by NextGen Creators."
    ];

    // Check if the user message matches any predefined response
    foreach ($responses as $key => $value) {
        if (strpos($userMessage, $key) !== false) {
            $response = $value;
            break;
        }
    }

    echo $response;
}
?>