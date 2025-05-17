<?php

class Category {
    private ?int $id;
    private string $name;
    private ?string $description;

    public function __construct(?int $id = null, string $name, ?string $description = null) {
        $this->id = $id;
        $this->name = $name;
        $this->description = $description;
    }

    // Getters
    public function getId(): ?int {
        return $this->id;
    }

    public function getName(): string {
        return $this->name;
    }

    public function getDescription(): ?string {
        return $this->description;
    }

    // Setters
    public function setName(string $name): void {
        $this->name = htmlspecialchars(trim($name));
    }

    public function setDescription(?string $description): void {
        $this->description = htmlspecialchars(trim($description));
    }
}

?>
