SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL';

CREATE SCHEMA IF NOT EXISTS `TAM_V3` DEFAULT CHARACTER SET latin1 COLLATE latin1_general_ci ;
USE `TAM_V3` ;

-- -----------------------------------------------------
-- Table `TAM_V3`.`Dilution`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `TAM_V3`.`Dilution` ;

CREATE  TABLE IF NOT EXISTS `TAM_V3`.`Dilution` (
  `etalon` VARCHAR(100) NOT NULL DEFAULT '' ,
  `date` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP ,
  `point` SMALLINT(6) NOT NULL DEFAULT '0' ,
  `point_etalon` SMALLINT(6) NOT NULL DEFAULT '0' ,
  `taux` FLOAT NOT NULL DEFAULT '0' ,
  PRIMARY KEY (`etalon`, `date`, `point`) )
ENGINE = MyISAM
DEFAULT CHARACTER SET = latin1;


-- -----------------------------------------------------
-- Table `TAM_V3`.`Marque_Equipement`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `TAM_V3`.`Marque_Equipement` ;

CREATE  TABLE IF NOT EXISTS `TAM_V3`.`Marque_Equipement` (
  `id_marque` SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `designation` VARCHAR(30) NOT NULL ,
  PRIMARY KEY (`id_marque`) )
ENGINE = MyISAM;

CREATE UNIQUE INDEX `designation_UNIQUE` ON `TAM_V3`.`Marque_Equipement` (`designation` ASC) ;


-- -----------------------------------------------------
-- Table `TAM_V3`.`Protocole`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `TAM_V3`.`Protocole` ;

CREATE  TABLE IF NOT EXISTS `TAM_V3`.`Protocole` (
  `id_Protocole` SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `designation` VARCHAR(20) NOT NULL ,
  PRIMARY KEY (`id_Protocole`) )
ENGINE = MyISAM;

CREATE UNIQUE INDEX `designation_UNIQUE` ON `TAM_V3`.`Protocole` (`designation` ASC) ;


-- -----------------------------------------------------
-- Table `TAM_V3`.`Modele_Equipement`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `TAM_V3`.`Modele_Equipement` ;

CREATE  TABLE IF NOT EXISTS `TAM_V3`.`Modele_Equipement` (
  `id_modele` SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `id_marque` SMALLINT UNSIGNED NOT NULL ,
  `id_protocole` SMALLINT UNSIGNED NOT NULL ,
  `designation` VARCHAR(45) NULL DEFAULT NULL ,
  PRIMARY KEY (`id_modele`) ,
  CONSTRAINT `fk_modele_marque`
    FOREIGN KEY (`id_marque` )
    REFERENCES `TAM_V3`.`Marque_Equipement` (`id_marque` )
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT `fk_modele_protocole`
    FOREIGN KEY (`id_protocole` )
    REFERENCES `TAM_V3`.`Protocole` (`id_Protocole` )
    ON DELETE RESTRICT
    ON UPDATE CASCADE)
ENGINE = MyISAM;

CREATE INDEX `fk_modele_marque` ON `TAM_V3`.`Modele_Equipement` (`id_marque` ASC) ;

CREATE INDEX `fk_modele_protocole` ON `TAM_V3`.`Modele_Equipement` (`id_protocole` ASC) ;


-- -----------------------------------------------------
-- Table `TAM_V3`.`Taux_Transmission`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `TAM_V3`.`Taux_Transmission` ;

CREATE  TABLE IF NOT EXISTS `TAM_V3`.`Taux_Transmission` (
  `id_tx_transmission` TINYINT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `taux_transmission` INT UNSIGNED NULL ,
  PRIMARY KEY (`id_tx_transmission`) )
ENGINE = MyISAM;

CREATE UNIQUE INDEX `taux_transmission_UNIQUE` ON `TAM_V3`.`Taux_Transmission` (`taux_transmission` ASC) ;


-- -----------------------------------------------------
-- Table `TAM_V3`.`Equipement`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `TAM_V3`.`Equipement` ;

CREATE  TABLE IF NOT EXISTS `TAM_V3`.`Equipement` (
  `id_equipement` SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `id_modele` SMALLINT NOT NULL ,
  `numero_serie` VARCHAR(45) NOT NULL ,
  `type` ENUM('ANALYSEUR','DILUTEUR','BOUTEILLE','GO3','GZERO') NOT NULL ,
  `en_service` TINYINT(1)  NOT NULL DEFAULT false ,
  `min_gamme` SMALLINT NULL DEFAULT 0 ,
  `max_gamme` SMALLINT NULL DEFAULT 0 ,
  `offset` SMALLINT NULL DEFAULT 0 ,
  `id_tx_transmission` TINYINT UNSIGNED NOT NULL ,
  `adresse` VARCHAR(10) NULL ,
  `nb_bits_transmission` TINYINT NULL DEFAULT 0 ,
  `nb_bits_stop` TINYINT NULL DEFAULT 0 ,
  `controle_flux` ENUM('AUCUN','HARDWARE','XON/XOFF') NULL DEFAULT 'AUCUN' ,
  `type_parite` ENUM('AUCUNE','ODD','EVEN','MARK','SPACE') NULL DEFAULT 'AUCUNE' ,
  PRIMARY KEY (`id_equipement`) ,
  CONSTRAINT `fk_eq_modele`
    FOREIGN KEY (`id_modele` )
    REFERENCES `TAM_V3`.`Modele_Equipement` (`id_modele` )
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT `fk_eq_tx_transmission`
    FOREIGN KEY (`id_tx_transmission` )
    REFERENCES `TAM_V3`.`Taux_Transmission` (`id_tx_transmission` )
    ON DELETE RESTRICT
    ON UPDATE CASCADE)
ENGINE = MyISAM
DEFAULT CHARACTER SET = latin1
COLLATE = latin1_general_ci;

CREATE INDEX `fk_eq_modele` ON `TAM_V3`.`Equipement` (`id_modele` ASC) ;

CREATE INDEX `fk_eq_tx_transmission` ON `TAM_V3`.`Equipement` (`id_tx_transmission` ASC) ;


-- -----------------------------------------------------
-- Table `TAM_V3`.`System_Etalonnage`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `TAM_V3`.`System_Etalonnage` ;

CREATE  TABLE IF NOT EXISTS `TAM_V3`.`System_Etalonnage` (
  `id_systeme_etalon` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `id_etalon` SMALLINT UNSIGNED NOT NULL ,
  `id_bouteille` SMALLINT UNSIGNED NULL DEFAULT 0 ,
  `id_gzero` SMALLINT UNSIGNED NULL DEFAULT 0 ,
  PRIMARY KEY (`id_systeme_etalon`) ,
  CONSTRAINT `fk_se_equipement`
    FOREIGN KEY (`id_etalon` , `id_bouteille` , `id_gzero` )
    REFERENCES `TAM_V3`.`Equipement` (`id_equipement` , `id_equipement` , `id_equipement` )
    ON DELETE RESTRICT
    ON UPDATE CASCADE)
ENGINE = MyISAM
AUTO_INCREMENT = 63;

CREATE INDEX `fk_se_equipement` ON `TAM_V3`.`System_Etalonnage` (`id_etalon` ASC, `id_bouteille` ASC, `id_gzero` ASC) ;


-- -----------------------------------------------------
-- Table `TAM_V3`.`Test_XML`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `TAM_V3`.`Test_XML` ;

CREATE  TABLE IF NOT EXISTS `TAM_V3`.`Test_XML` (
  `id_Test_Xml` SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `designation` VARCHAR(45) NOT NULL ,
  `fichier_description` VARCHAR(45) NOT NULL ,
  `type_test` ENUM('REPETABILITE_1','REPETABILITE_2','LINEARITE','TEMPS_REPONSE','TPG','AUTRE') NOT NULL DEFAULT 'AUTRE' ,
  PRIMARY KEY (`id_Test_Xml`) )
ENGINE = MyISAM;

CREATE UNIQUE INDEX `fichier_description_UNIQUE` ON `TAM_V3`.`Test_XML` (`fichier_description` ASC) ;


-- -----------------------------------------------------
-- Table `TAM_V3`.`Operateur`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `TAM_V3`.`Operateur` ;

CREATE  TABLE IF NOT EXISTS `TAM_V3`.`Operateur` (
  `id_operateur` SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `Nom` VARCHAR(30) NOT NULL ,
  `Prenom` VARCHAR(30) NOT NULL ,
  PRIMARY KEY (`id_operateur`) )
ENGINE = MyISAM
AUTO_INCREMENT = 28;


-- -----------------------------------------------------
-- Table `TAM_V3`.`Lieu`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `TAM_V3`.`Lieu` ;

CREATE  TABLE IF NOT EXISTS `TAM_V3`.`Lieu` (
  `id_lieu` SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `designation` VARCHAR(45) NOT NULL ,
  PRIMARY KEY (`id_lieu`) )
ENGINE = MyISAM;


-- -----------------------------------------------------
-- Table `TAM_V3`.`Test_Metrologique`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `TAM_V3`.`Test_Metrologique` ;

CREATE  TABLE IF NOT EXISTS `TAM_V3`.`Test_Metrologique` (
  `id_test` SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `id_description_xml` SMALLINT UNSIGNED NOT NULL ,
  `id_operateur` SMALLINT UNSIGNED NOT NULL ,
  `id_systeme_etalon` INT UNSIGNED NOT NULL ,
  `id_lieu` SMALLINT UNSIGNED NOT NULL ,
  `pression` INT(11)  NULL DEFAULT NULL ,
  `debit` DECIMAL(10,0)  NULL DEFAULT NULL ,
  `temperature` DECIMAL(10,0)  NULL DEFAULT NULL ,
  `date_debut` DATETIME NOT NULL ,
  `date_fin` DATETIME NOT NULL ,
  `max_gamme` INT(11) NOT NULL DEFAULT '0' ,
  `min_gamme` INT(11) NOT NULL DEFAULT '0' ,
  `offset` INT(11) NOT NULL DEFAULT '0' ,
  PRIMARY KEY (`id_test`) ,
  CONSTRAINT `fk_test_systeme_etalon`
    FOREIGN KEY (`id_systeme_etalon` )
    REFERENCES `TAM_V3`.`System_Etalonnage` (`id_systeme_etalon` )
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT `fk_test_xml`
    FOREIGN KEY (`id_description_xml` )
    REFERENCES `TAM_V3`.`Test_XML` (`id_Test_Xml` )
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT `fk_test_operateur`
    FOREIGN KEY (`id_operateur` )
    REFERENCES `TAM_V3`.`Operateur` (`id_operateur` )
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT `fk_test_lieu`
    FOREIGN KEY (`id_lieu` )
    REFERENCES `TAM_V3`.`Lieu` (`id_lieu` )
    ON DELETE RESTRICT
    ON UPDATE CASCADE)
ENGINE = MyISAM
AUTO_INCREMENT = 2455;

CREATE INDEX `fk_test_systeme_etalon` ON `TAM_V3`.`Test_Metrologique` (`id_systeme_etalon` ASC) ;

CREATE INDEX `fk_test_xml` ON `TAM_V3`.`Test_Metrologique` (`id_description_xml` ASC) ;

CREATE INDEX `fk_test_operateur` ON `TAM_V3`.`Test_Metrologique` (`id_operateur` ASC) ;

CREATE INDEX `fk_test_lieu` ON `TAM_V3`.`Test_Metrologique` (`id_lieu` ASC) ;


-- -----------------------------------------------------
-- Table `TAM_V3`.`Mesure`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `TAM_V3`.`Mesure` ;

CREATE  TABLE IF NOT EXISTS `TAM_V3`.`Mesure` (
  `no_mesure` SMALLINT UNSIGNED NOT NULL ,
  `id_test` SMALLINT UNSIGNED NOT NULL ,
  `id_equipement` SMALLINT UNSIGNED NOT NULL ,
  `mesure` DECIMAL(10,0)  NOT NULL ,
  `mesure_moyenne` TINYINT(1)  NOT NULL DEFAULT false ,
  PRIMARY KEY (`no_mesure`, `id_test`, `id_equipement`) ,
  CONSTRAINT `fk_mesure_test`
    FOREIGN KEY (`id_test` )
    REFERENCES `TAM_V3`.`Test_Metrologique` (`id_test` )
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT `fk_mesure_equipement`
    FOREIGN KEY (`id_equipement` )
    REFERENCES `TAM_V3`.`Equipement` (`id_equipement` )
    ON DELETE RESTRICT
    ON UPDATE CASCADE)
ENGINE = MyISAM;

CREATE INDEX `fk_mesure_test` ON `TAM_V3`.`Mesure` (`id_test` ASC) ;

CREATE INDEX `fk_mesure_equipement` ON `TAM_V3`.`Mesure` (`id_equipement` ASC) ;


-- -----------------------------------------------------
-- Table `TAM_V3`.`Port_Serie`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `TAM_V3`.`Port_Serie` ;

CREATE  TABLE IF NOT EXISTS `TAM_V3`.`Port_Serie` (
  `no_port` INT(2) UNSIGNED NOT NULL DEFAULT '0' ,
  `designation` VARCHAR(20) NULL DEFAULT NULL ,
  PRIMARY KEY (`no_port`) )
ENGINE = MyISAM;


-- -----------------------------------------------------
-- Table `TAM_V3`.`Molecule`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `TAM_V3`.`Molecule` ;

CREATE  TABLE IF NOT EXISTS `TAM_V3`.`Molecule` (
  `id_molecule` SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `code` VARCHAR(5) NOT NULL ,
  `nom` VARCHAR(50) NOT NULL ,
  `formule` VARCHAR(10) NOT NULL ,
  PRIMARY KEY (`id_molecule`) )
ENGINE = MyISAM;

CREATE UNIQUE INDEX `code_UNIQUE` ON `TAM_V3`.`Molecule` (`code` ASC) ;

CREATE UNIQUE INDEX `formule_UNIQUE` ON `TAM_V3`.`Molecule` (`formule` ASC) ;


-- -----------------------------------------------------
-- Table `TAM_V3`.`Polluant_Associe`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `TAM_V3`.`Polluant_Associe` ;

CREATE  TABLE IF NOT EXISTS `TAM_V3`.`Polluant_Associe` (
  `id_polluant_associe` INT NOT NULL AUTO_INCREMENT ,
  `id_pa_equipement` SMALLINT UNSIGNED NOT NULL ,
  `id_pa_molecule` SMALLINT UNSIGNED NOT NULL ,
  PRIMARY KEY (`id_polluant_associe`) ,
  CONSTRAINT `fk_po_equipement`
    FOREIGN KEY (`id_pa_equipement` )
    REFERENCES `TAM_V3`.`Equipement` (`id_equipement` )
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT `fk_po_molecule`
    FOREIGN KEY (`id_pa_molecule` )
    REFERENCES `TAM_V3`.`Molecule` (`id_molecule` )
    ON DELETE RESTRICT
    ON UPDATE CASCADE)
ENGINE = MyISAM;

CREATE INDEX `fk_po_equipement` ON `TAM_V3`.`Polluant_Associe` (`id_pa_equipement` ASC) ;

CREATE INDEX `fk_po_molecule` ON `TAM_V3`.`Polluant_Associe` (`id_pa_molecule` ASC) ;


-- -----------------------------------------------------
-- Table `TAM_V3`.`Concentration`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `TAM_V3`.`Concentration` ;

CREATE  TABLE IF NOT EXISTS `TAM_V3`.`Concentration` (
  `id_Concentration` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `id_systeme_etalon` INT UNSIGNED NOT NULL ,
  `id_molecule` SMALLINT UNSIGNED NOT NULL ,
  `point_consigne` SMALLINT NOT NULL ,
  `conc_reelle` SMALLINT UNSIGNED NOT NULL ,
  `conc_ozone` SMALLINT UNSIGNED NULL DEFAULT 0 ,
  PRIMARY KEY (`id_Concentration`) ,
  CONSTRAINT `fk_system_etalon`
    FOREIGN KEY (`id_systeme_etalon` )
    REFERENCES `TAM_V3`.`System_Etalonnage` (`id_systeme_etalon` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = MyISAM;

CREATE INDEX `fk_system_etalon` USING BTREE ON `TAM_V3`.`Concentration` (`id_systeme_etalon` ASC) ;


-- -----------------------------------------------------
-- Table `TAM_V3`.`Liste_Analyseurs_Test`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `TAM_V3`.`Liste_Analyseurs_Test` ;

CREATE  TABLE IF NOT EXISTS `TAM_V3`.`Liste_Analyseurs_Test` (
  `id_test` SMALLINT UNSIGNED NOT NULL ,
  `id_equipement` SMALLINT UNSIGNED NOT NULL ,
  PRIMARY KEY (`id_test`, `id_equipement`) ,
  CONSTRAINT `fk_liste_ana_test`
    FOREIGN KEY (`id_test` )
    REFERENCES `TAM_V3`.`Test_Metrologique` (`id_test` )
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT `fk_liste_ana_equipement`
    FOREIGN KEY (`id_equipement` )
    REFERENCES `TAM_V3`.`Equipement` (`id_equipement` )
    ON DELETE RESTRICT
    ON UPDATE CASCADE)
ENGINE = MyISAM;

CREATE INDEX `fk_liste_ana_test` ON `TAM_V3`.`Liste_Analyseurs_Test` (`id_test` ASC) ;

CREATE INDEX `fk_liste_ana_equipement` ON `TAM_V3`.`Liste_Analyseurs_Test` (`id_equipement` ASC) ;


-- -----------------------------------------------------
-- Table `TAM_V3`.`Equipement_Reforme`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `TAM_V3`.`Equipement_Reforme` ;

CREATE  TABLE IF NOT EXISTS `TAM_V3`.`Equipement_Reforme` (
  `id_equipement_reforme` SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `id_modele` SMALLINT UNSIGNED NOT NULL ,
  `id_equipement` SMALLINT UNSIGNED NOT NULL ,
  `date_reforme` VARCHAR(45) NOT NULL ,
  `numero_serie` VARCHAR(45) NOT NULL ,
  PRIMARY KEY (`id_equipement_reforme`) ,
  CONSTRAINT `fk_eq_reforme_modele`
    FOREIGN KEY (`id_modele` )
    REFERENCES `TAM_V3`.`Modele_Equipement` (`id_modele` )
    ON DELETE RESTRICT
    ON UPDATE CASCADE)
ENGINE = MyISAM;

CREATE UNIQUE INDEX `id_equipement_UNIQUE` ON `TAM_V3`.`Equipement_Reforme` (`id_equipement` ASC) ;

CREATE INDEX `fk_eq_reforme_modele` ON `TAM_V3`.`Equipement_Reforme` (`id_modele` ASC) ;



SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;

-- -----------------------------------------------------
-- Data for table `TAM_V3`.`Marque_Equipement`
-- -----------------------------------------------------
START TRANSACTION;
USE `TAM_V3`;
INSERT INTO `TAM_V3`.`Marque_Equipement` (`id_marque`, `designation`) VALUES (9, 'LNI');
INSERT INTO `TAM_V3`.`Marque_Equipement` (`id_marque`, `designation`) VALUES (3, 'AIRLI');
INSERT INTO `TAM_V3`.`Marque_Equipement` (`id_marque`, `designation`) VALUES (10, 'MESSER');
INSERT INTO `TAM_V3`.`Marque_Equipement` (`id_marque`, `designation`) VALUES (11, 'SERES');
INSERT INTO `TAM_V3`.`Marque_Equipement` (`id_marque`, `designation`) VALUES (7, 'ENVSA');
INSERT INTO `TAM_V3`.`Marque_Equipement` (`id_marque`, `designation`) VALUES (13, 'TEI');
INSERT INTO `TAM_V3`.`Marque_Equipement` (`id_marque`, `designation`) VALUES (6, 'CHROMATO-SUD');
INSERT INTO `TAM_V3`.`Marque_Equipement` (`id_marque`, `designation`) VALUES (12, 'SYNSPEC');
INSERT INTO `TAM_V3`.`Marque_Equipement` (`id_marque`, `designation`) VALUES (5, 'API');
INSERT INTO `TAM_V3`.`Marque_Equipement` (`id_marque`, `designation`) VALUES (2, 'AIRLIQUIDE');
INSERT INTO `TAM_V3`.`Marque_Equipement` (`id_marque`, `designation`) VALUES (8, 'HORIBA');
INSERT INTO `TAM_V3`.`Marque_Equipement` (`id_marque`, `designation`) VALUES (4, 'ANSYCO');
INSERT INTO `TAM_V3`.`Marque_Equipement` (`id_marque`, `designation`) VALUES (1, 'INCONNUE');

COMMIT;

-- -----------------------------------------------------
-- Data for table `TAM_V3`.`Protocole`
-- -----------------------------------------------------
START TRANSACTION;
USE `TAM_V3`;
INSERT INTO `TAM_V3`.`Protocole` (`id_Protocole`, `designation`) VALUES (1, 'INCONNU');

COMMIT;

-- -----------------------------------------------------
-- Data for table `TAM_V3`.`Modele_Equipement`
-- -----------------------------------------------------
START TRANSACTION;
USE `TAM_V3`;
INSERT INTO `TAM_V3`.`Modele_Equipement` (`id_modele`, `id_marque`, `id_protocole`, `designation`) VALUES (1, 1, 1, 'INCONNU');

COMMIT;

-- -----------------------------------------------------
-- Data for table `TAM_V3`.`Taux_Transmission`
-- -----------------------------------------------------
START TRANSACTION;
USE `TAM_V3`;
INSERT INTO `TAM_V3`.`Taux_Transmission` (`id_tx_transmission`, `taux_transmission`) VALUES (NULL, 50);
INSERT INTO `TAM_V3`.`Taux_Transmission` (`id_tx_transmission`, `taux_transmission`) VALUES (NULL, 75);
INSERT INTO `TAM_V3`.`Taux_Transmission` (`id_tx_transmission`, `taux_transmission`) VALUES (NULL, 110);
INSERT INTO `TAM_V3`.`Taux_Transmission` (`id_tx_transmission`, `taux_transmission`) VALUES (NULL, 134);
INSERT INTO `TAM_V3`.`Taux_Transmission` (`id_tx_transmission`, `taux_transmission`) VALUES (NULL, 150);
INSERT INTO `TAM_V3`.`Taux_Transmission` (`id_tx_transmission`, `taux_transmission`) VALUES (NULL, 200);
INSERT INTO `TAM_V3`.`Taux_Transmission` (`id_tx_transmission`, `taux_transmission`) VALUES (NULL, 300);
INSERT INTO `TAM_V3`.`Taux_Transmission` (`id_tx_transmission`, `taux_transmission`) VALUES (NULL, 600);
INSERT INTO `TAM_V3`.`Taux_Transmission` (`id_tx_transmission`, `taux_transmission`) VALUES (NULL, 1200);
INSERT INTO `TAM_V3`.`Taux_Transmission` (`id_tx_transmission`, `taux_transmission`) VALUES (NULL, 1800);
INSERT INTO `TAM_V3`.`Taux_Transmission` (`id_tx_transmission`, `taux_transmission`) VALUES (NULL, 2400);
INSERT INTO `TAM_V3`.`Taux_Transmission` (`id_tx_transmission`, `taux_transmission`) VALUES (NULL, 4800);
INSERT INTO `TAM_V3`.`Taux_Transmission` (`id_tx_transmission`, `taux_transmission`) VALUES (NULL, 9600);
INSERT INTO `TAM_V3`.`Taux_Transmission` (`id_tx_transmission`, `taux_transmission`) VALUES (NULL, 14400);
INSERT INTO `TAM_V3`.`Taux_Transmission` (`id_tx_transmission`, `taux_transmission`) VALUES (NULL, 19200);
INSERT INTO `TAM_V3`.`Taux_Transmission` (`id_tx_transmission`, `taux_transmission`) VALUES (NULL, 38400);
INSERT INTO `TAM_V3`.`Taux_Transmission` (`id_tx_transmission`, `taux_transmission`) VALUES (NULL, 56000);
INSERT INTO `TAM_V3`.`Taux_Transmission` (`id_tx_transmission`, `taux_transmission`) VALUES (NULL, 57600);
INSERT INTO `TAM_V3`.`Taux_Transmission` (`id_tx_transmission`, `taux_transmission`) VALUES (NULL, 76800);
INSERT INTO `TAM_V3`.`Taux_Transmission` (`id_tx_transmission`, `taux_transmission`) VALUES (NULL, 115200);
INSERT INTO `TAM_V3`.`Taux_Transmission` (`id_tx_transmission`, `taux_transmission`) VALUES (NULL, 128000);

COMMIT;

-- -----------------------------------------------------
-- Data for table `TAM_V3`.`Equipement`
-- -----------------------------------------------------
START TRANSACTION;
USE `TAM_V3`;
INSERT INTO `TAM_V3`.`Equipement` (`id_equipement`, `id_modele`, `numero_serie`, `type`, `en_service`, `min_gamme`, `max_gamme`, `offset`, `id_tx_transmission`, `adresse`, `nb_bits_transmission`, `nb_bits_stop`, `controle_flux`, `type_parite`) VALUES (1, 1, 'vide', 'BOUTEILLE', 1, NULL, NULL, NULL, 1, NULL, NULL, NULL, NULL, NULL);
INSERT INTO `TAM_V3`.`Equipement` (`id_equipement`, `id_modele`, `numero_serie`, `type`, `en_service`, `min_gamme`, `max_gamme`, `offset`, `id_tx_transmission`, `adresse`, `nb_bits_transmission`, `nb_bits_stop`, `controle_flux`, `type_parite`) VALUES (2, 1, 'vide', 'GZERO', 1, NULL, NULL, NULL, 1, NULL, NULL, NULL, NULL, NULL);

COMMIT;

-- -----------------------------------------------------
-- Data for table `TAM_V3`.`Lieu`
-- -----------------------------------------------------
START TRANSACTION;
USE `TAM_V3`;
INSERT INTO `TAM_V3`.`Lieu` (`id_lieu`, `designation`) VALUES (NULL, 'Laboratoire métrologique ATMO poito-charentes');

COMMIT;

-- -----------------------------------------------------
-- Data for table `TAM_V3`.`Molecule`
-- -----------------------------------------------------
START TRANSACTION;
USE `TAM_V3`;
INSERT INTO `TAM_V3`.`Molecule` (`id_molecule`, `code`, `nom`, `formule`) VALUES (NULL, '1', 'Dioxyde de soufre', 'SO2');
INSERT INTO `TAM_V3`.`Molecule` (`id_molecule`, `code`, `nom`, `formule`) VALUES (NULL, '2', 'Monoxyde d\'azote', 'NO');
INSERT INTO `TAM_V3`.`Molecule` (`id_molecule`, `code`, `nom`, `formule`) VALUES (NULL, '3', 'Dioxyde d\'azote', 'NO2');
INSERT INTO `TAM_V3`.`Molecule` (`id_molecule`, `code`, `nom`, `formule`) VALUES (NULL, '4', 'Monoxyde de carbone', 'CO');
INSERT INTO `TAM_V3`.`Molecule` (`id_molecule`, `code`, `nom`, `formule`) VALUES (NULL, '8', 'Ozone', 'O3');
INSERT INTO `TAM_V3`.`Molecule` (`id_molecule`, `code`, `nom`, `formule`) VALUES (NULL, '12', 'Oxydes d\'azote', 'NOX');

COMMIT;
