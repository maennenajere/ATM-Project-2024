-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';

-- -----------------------------------------------------
-- Schema automaatti
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema automaatti
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `automaatti` DEFAULT CHARACTER SET utf8 ;
USE `automaatti` ;

-- -----------------------------------------------------
-- Table `automaatti`.`customer`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `automaatti`.`customer` (
  `idcustomer` INT NOT NULL AUTO_INCREMENT,
  `customerName` VARCHAR(45) NULL,
  `customerAddress` VARCHAR(45) NULL,
  `customerPhonenumber` VARCHAR(45) NULL,
  PRIMARY KEY (`idcustomer`),
  UNIQUE INDEX `customerPhonenumber_UNIQUE` (`customerPhonenumber` ASC) VISIBLE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `automaatti`.`card`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `automaatti`.`card` (
  `idcard` INT NOT NULL AUTO_INCREMENT,
  `idaccount` INT NOT NULL,
  `idcustomer` INT NOT NULL,
  `cardNumber` VARCHAR(45) NULL,
  `cardPin` VARCHAR(255) NULL,
  PRIMARY KEY (`idcard`, `idaccount`, `idcustomer`),
  UNIQUE INDEX `cardNumber_UNIQUE` (`cardNumber` ASC) VISIBLE,
  INDEX `card-account_idx` (`idcustomer` ASC) VISIBLE,
  CONSTRAINT `customer-card`
    FOREIGN KEY (`idcustomer`)
    REFERENCES `automaatti`.`customer` (`idcustomer`)
    ON DELETE RESTRICT
    ON UPDATE CASCADE)
ENGINE = InnoDB
KEY_BLOCK_SIZE = 2;


-- -----------------------------------------------------
-- Table `automaatti`.`account`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `automaatti`.`account` (
  `idaccount` INT NOT NULL AUTO_INCREMENT,
  `accountNumber` VARCHAR(45) NULL,
  `accountType` VARCHAR(45) NULL,
  `accountBalance` DECIMAL(10,2) NULL,
  `creditLimit` DECIMAL(10,2) NULL,
  PRIMARY KEY (`idaccount`),
  UNIQUE INDEX `accountNumber_UNIQUE` (`accountNumber` ASC) VISIBLE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `automaatti`.`transactions`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `automaatti`.`transactions` (
  `idtransactions` INT NOT NULL AUTO_INCREMENT,
  `idaccount` INT NOT NULL,
  `transactionsDate` DATETIME NULL,
  `transactionsType` VARCHAR(45) NULL,
  `transactionsAmount` DECIMAL(10,2) NULL,
  PRIMARY KEY (`idtransactions`, `idaccount`),
  INDEX `transactions-account_idx` (`idaccount` ASC) VISIBLE,
  CONSTRAINT `transactions-account`
    FOREIGN KEY (`idaccount`)
    REFERENCES `automaatti`.`account` (`idaccount`)
    ON DELETE RESTRICT
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `automaatti`.`card-account`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `automaatti`.`card-account` (
  `idcard` INT NOT NULL,
  `idaccount` INT NOT NULL,
  PRIMARY KEY (`idcard`, `idaccount`),
  INDEX `(card-account)-account_idx` (`idaccount` ASC) VISIBLE,
  CONSTRAINT `(card-account)-card`
    FOREIGN KEY (`idcard`)
    REFERENCES `automaatti`.`card` (`idcard`)
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `(card-account)-account`
    FOREIGN KEY (`idaccount`)
    REFERENCES `automaatti`.`account` (`idaccount`)
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
