<?php

class user {
    private ?int $id;
    private ?string $Name;
    private ?string $surname;
    private ?string $Email;
    private ?string $password;

    // Constructor
    public function __construct(?int $id, ?string $Name, ?string $surname,?string $Email,  ?string $password) {
        $this->id = $id;
        $this->Name = $Name;
        $this->surname= $surname;
        $this->Email = $Email;
        $this->password = $password;
    }

    // Getters and Setters

    public function getId(): ?int {
        return $this->id;
    }

    public function setId(?int $id): void {
        $this->id = $id;
    }

    public function getName(): ?string {
        return $this->Name;
    }

    public function setName(?string $Name): void {
        $this->Name = $Name;
    }

    public function getsurname(): ?string {
        return $this->surname;
    }

    public function setsurname(?string $surname): void {
        $this->surname = $surname;
    }

    
    public function getEmail(): string {
        return $this->Email;
    }

    public function setEmail(string $Email): void {
        $this->Email = $Email;
    }

    public function getpassword(): string {
        return $this->password;
    }

    public function setpassword(string $password): void {
        $this->password = $password;
    }
}

?>
