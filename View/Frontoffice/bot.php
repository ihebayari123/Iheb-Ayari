

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Chatbot Test</title>
    <link rel="stylesheet" href="style3.css"></link>
</head>
<body>

<div id="chatbot-container">
    <div id="chatbot-header">
        <h3>Chatbot</h3>
        <button id="chatbot-close">×</button>
    </div>
    <div id="chatbot-messages">
        <div class="chatbot-message chatbot-welcome">
            <p>Hello! How can I assist you today?</p>
        </div>
    </div>
    <div id="chatbot-input-container">
        <input type="text" id="chatbot-input" placeholder="Type your message...">
        <button id="chatbot-send">Send</button>
    </div>
</div>
<button id="chatbot-toggle">
    <img src="images/chatbot.png" alt="Chatbot" width="50" height="50">
</button>


<script>
    document.addEventListener('DOMContentLoaded', function () {
        const chatbotContainer = document.getElementById('chatbot-container');
        const chatbotToggle = document.getElementById('chatbot-toggle');

        chatbotToggle.addEventListener('click', function () {
            if (chatbotContainer.style.display === 'block') {
                chatbotContainer.style.display = 'none';
            } else {
                chatbotContainer.style.display = 'block';
            }
        });

        // Close chatbot on clicking the close button
        const chatbotClose = document.getElementById('chatbot-close');
        chatbotClose.addEventListener('click', function () {
            chatbotContainer.style.display = 'none';
        });
    });
</script>

<script>
    document.addEventListener('DOMContentLoaded', function () {
    const chatbotInput = document.getElementById('chatbot-input');
    const chatbotSend = document.getElementById('chatbot-send');
    const chatbotMessages = document.getElementById('chatbot-messages');

    chatbotSend.addEventListener('click', function () {
        const userMessage = chatbotInput.value.trim();
        if (userMessage === "") return;

        // Display user message
        const userMessageDiv = document.createElement('div');
        userMessageDiv.classList.add('chatbot-message', 'user-message');
        userMessageDiv.innerHTML = `<p>${userMessage}</p>`;
        chatbotMessages.appendChild(userMessageDiv);

        // Clear input field
        chatbotInput.value = "";

        // Send the message to the server
        const xhr = new XMLHttpRequest();
        xhr.open('POST', 'chatbot_response.php', true);
        xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
        xhr.onload = function () {
            if (xhr.status === 200) {
                const botResponseDiv = document.createElement('div');
                botResponseDiv.classList.add('chatbot-message', 'bot-message');
                botResponseDiv.innerHTML = `<p>${xhr.responseText}</p>`;
                chatbotMessages.appendChild(botResponseDiv);

                // Scroll to the latest message
                chatbotMessages.scrollTop = chatbotMessages.scrollHeight;
            }
        };
        xhr.send(`message=${encodeURIComponent(userMessage)}`);
    });
});

</script>


</body>
</html>
