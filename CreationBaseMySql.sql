-- --------------------------------------------------------
-- Hôte:                         127.0.0.1
-- Version du serveur:           10.6.7-MariaDB - mariadb.org binary distribution
-- SE du serveur:                Win64
-- HeidiSQL Version:             11.3.0.6295
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

-- Listage de la structure de la table tam_rpi. concentration

CREATE TABLE IF NOT EXISTS `Concentration` (
  `id_Concentration` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `id_systeme_etalon` int(10) unsigned NOT NULL,
  `id_molecule` smallint(5) unsigned NOT NULL,
  `point_consigne` int(10) NOT NULL,
  `conc_reelle` decimal(10,2) NOT NULL,
  `conc_ozone` smallint(5) unsigned NOT NULL DEFAULT 0,
  `archivee` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`id_Concentration`),
  UNIQUE KEY `fk_Concentration_Systeme_Etalon` (`id_systeme_etalon`,`id_molecule`,`point_consigne`,`conc_reelle`),
  KEY `fk_Molecule` (`id_molecule`),
  CONSTRAINT `fk_Concentration_Systeme_Etalon` FOREIGN KEY (`id_systeme_etalon`) REFERENCES `systeme_etalonnage` (`id_systeme_etalon`) ON UPDATE CASCADE,
  CONSTRAINT `fk_Molecule` FOREIGN KEY (`id_molecule`) REFERENCES `molecule` (`id_molecule`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=284 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Les données exportées n'étaient pas sélectionnées.

-- Listage de la structure de la table tam_rpi. concentration_associee

CREATE TABLE IF NOT EXISTS `Concentration_Associee` (
  `id_concentration_associee` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `id_concentration` int(10) unsigned NOT NULL,
  `id_molecule` smallint(5) unsigned NOT NULL,
  `concentration` decimal(10,2) unsigned NOT NULL,
  PRIMARY KEY (`id_concentration_associee`),
  UNIQUE KEY `fk_Concentration_Associee_id_Concentration` (`id_concentration`,`id_molecule`),
  KEY `fk_Concentration_Associee_Concentration` (`concentration`),
  KEY `fk_Concentration_Associee_Molecule` (`id_molecule`),
  CONSTRAINT `fk_Concentration_Associee_Concentration` FOREIGN KEY (`id_concentration`) REFERENCES `concentration` (`id_Concentration`) ON UPDATE CASCADE,
  CONSTRAINT `fk_Concentration_Associee_Molecule` FOREIGN KEY (`id_molecule`) REFERENCES `molecule` (`id_molecule`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=103 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Les données exportées n'étaient pas sélectionnées.

-- Listage de la structure de la table tam_rpi. concentration_test_metro

CREATE TABLE IF NOT EXISTS `Concentration_Test_Metro` (
  `idConcentration_Test_Metro` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `id_test` smallint(5) unsigned NOT NULL,
  `noPhase` tinyint(3) unsigned NOT NULL,
  `id_molecule` smallint(5) unsigned NOT NULL,
  `concentration` decimal(10,2) NOT NULL,
  `nbre_acquisition` smallint(5) NOT NULL DEFAULT 0,
  PRIMARY KEY (`idConcentration_Test_Metro`),
  KEY `fk_Concentration_Test_Metro_1` (`id_test`),
  KEY `fk_Concentration_id_molecule` (`id_molecule`),
  CONSTRAINT `Concentration_Test_Metro_ibfk_1` FOREIGN KEY (`id_molecule`) REFERENCES `molecule` (`id_molecule`) ON UPDATE CASCADE,
  CONSTRAINT `fk_Concentration_Test_Metro_1` FOREIGN KEY (`id_test`) REFERENCES `test_metrologique` (`id_test`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=20688 DEFAULT CHARSET=latin1;


-- Les données exportées n'étaient pas sélectionnées.

-- Listage de la structure de la table tam_rpi. equipement
CREATE TABLE IF NOT EXISTS `Equipement` (
  `id_equipement` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `id_modele` smallint(5) unsigned NOT NULL,
  `numero_serie` varchar(45) COLLATE latin1_general_ci NOT NULL,
  `en_service` tinyint(1) NOT NULL DEFAULT 0,
  `min_gamme` int(11) DEFAULT 0,
  `max_gamme` int(11) DEFAULT 0,
  `offset` int(11) DEFAULT 0,
  `id_tx_transmission` tinyint(3) unsigned NOT NULL,
  `adresse` varchar(10) COLLATE latin1_general_ci DEFAULT NULL,
  `nb_bits_transmission` tinyint(4) DEFAULT 0,
  `nb_bits_stop` tinyint(4) DEFAULT 0,
  `controle_flux` enum('AUCUN','HARDWARE','XON/XOFF') COLLATE latin1_general_ci DEFAULT 'AUCUN',
  `type_parite` enum('AUCUNE','ODD','EVEN','MARK','SPACE') COLLATE latin1_general_ci DEFAULT 'AUCUNE',
  `no_port` smallint(6) DEFAULT 0,
  `ip_adress` varchar(45) COLLATE latin1_general_ci DEFAULT '0.0.0.0',
  `num_port_IP` int(11) DEFAULT 0,
  `current_connexion_type` enum('IP','RS232') COLLATE latin1_general_ci DEFAULT 'RS232',
  `typeSocket` enum('UDP','TCP') COLLATE latin1_general_ci DEFAULT 'UDP',
  PRIMARY KEY (`id_equipement`),
  UNIQUE KEY `numero_serie_UNIQUE` (`numero_serie`),
  KEY `fk_eq_tx_transmission` (`id_tx_transmission`),
  KEY `fk_eq_modele` (`id_modele`),
  KEY `fk_eq_port_serie` (`no_port`),
  CONSTRAINT `fk_eq_modele` FOREIGN KEY (`id_modele`) REFERENCES `modele_equipement` (`id_modele`) ON UPDATE CASCADE,
  CONSTRAINT `fk_eq_port_serie` FOREIGN KEY (`no_port`) REFERENCES `port_serie` (`no_port`) ON UPDATE CASCADE,
  CONSTRAINT `fk_eq_tx_transmission` FOREIGN KEY (`id_tx_transmission`) REFERENCES `taux_transmission` (`id_tx_transmission`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=131 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Les données exportées n'étaient pas sélectionnées.

-- Listage de la structure de la table tam_rpi. equipement_reforme
CREATE TABLE IF NOT EXISTS `Equipement_Reforme` (
  `id_equipement_reforme` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `id_modele` smallint(5) unsigned NOT NULL,
  `id_equipement` smallint(5) unsigned NOT NULL,
  `date_reforme` varchar(45) COLLATE latin1_general_ci NOT NULL,
  `numero_serie` varchar(45) COLLATE latin1_general_ci NOT NULL,
  PRIMARY KEY (`id_equipement_reforme`),
  UNIQUE KEY `id_equipement_UNIQUE` (`id_equipement`),
  KEY `fk_eq_reforme_modele` (`id_modele`),
  CONSTRAINT `fk_eq_reforme_modele` FOREIGN KEY (`id_modele`) REFERENCES `modele_equipement` (`id_modele`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Les données exportées n'étaient pas sélectionnées.

-- Listage de la structure de la table tam_rpi. lieu
CREATE TABLE IF NOT EXISTS `Lieu` (
  `id_lieu` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `designation` varchar(45) COLLATE latin1_general_ci NOT NULL,
  PRIMARY KEY (`id_lieu`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Les données exportées n'étaient pas sélectionnées.

-- Listage de la structure de la table tam_rpi. liste_analyseurs_test
CREATE TABLE IF NOT EXISTS `Liste_Analyseurs_Test` (
  `id_test` smallint(5) unsigned NOT NULL,
  `id_equipement` smallint(5) unsigned NOT NULL,
  PRIMARY KEY (`id_test`,`id_equipement`),
  KEY `fk_liste_ana_test` (`id_test`),
  KEY `fk_liste_ana_equipement` (`id_equipement`),
  CONSTRAINT `fk_liste_ana_equipement` FOREIGN KEY (`id_equipement`) REFERENCES `equipement` (`id_equipement`) ON UPDATE CASCADE,
  CONSTRAINT `fk_liste_ana_test` FOREIGN KEY (`id_test`) REFERENCES `test_metrologique` (`id_test`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Les données exportées n'étaient pas sélectionnées.

-- Listage de la structure de la table tam_rpi. marque_equipement
CREATE TABLE IF NOT EXISTS `Marque_Equipement` (
  `id_marque` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `designation` varchar(30) COLLATE latin1_general_ci NOT NULL,
  PRIMARY KEY (`id_marque`),
  UNIQUE KEY `designation_UNIQUE` (`designation`)
) ENGINE=InnoDB AUTO_INCREMENT=17 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Les données exportées n'étaient pas sélectionnées.

-- Listage de la structure de la table tam_rpi. mesure
CREATE TABLE IF NOT EXISTS `Mesure` (
  `id_mesure` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `id_test` smallint(5) unsigned NOT NULL,
  `id_equipement` smallint(5) unsigned NOT NULL,
  `no_cyclePhase` smallint(6) DEFAULT NULL,
  `no_phase` smallint(6) DEFAULT NULL,
  `no_cycleMesure` smallint(6) DEFAULT NULL,
  `mesure_1` decimal(10,2) DEFAULT NULL,
  `mesure_2` decimal(10,2) DEFAULT NULL,
  `mesure_3` decimal(10,2) DEFAULT NULL,
  PRIMARY KEY (`id_mesure`,`id_test`,`id_equipement`),
  KEY `fk_mesure_test` (`id_test`),
  KEY `fk_mesure_equipement` (`id_equipement`),
  CONSTRAINT `fk_mesure_equipement` FOREIGN KEY (`id_equipement`) REFERENCES `equipement` (`id_equipement`) ON UPDATE CASCADE,
  CONSTRAINT `fk_mesure_test` FOREIGN KEY (`id_test`) REFERENCES `test_metrologique` (`id_test`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=289648 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;


-- Les données exportées n'étaient pas sélectionnées.

-- Listage de la structure de la table tam_rpi. modele_equipement
CREATE TABLE IF NOT EXISTS `Modele_Equipement` (
  `id_modele` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `id_marque` smallint(5) unsigned NOT NULL,
  `id_protocole` smallint(5) unsigned NOT NULL,
  `me_designation` varchar(45) COLLATE latin1_general_ci DEFAULT NULL,
  `type` enum('ANALYSEUR','DILUTEUR','BOUTEILLE','GO3','GZERO','SONDE') COLLATE latin1_general_ci DEFAULT NULL,
  PRIMARY KEY (`id_modele`),
  KEY `fk_Modele_Equipement_Marque` (`id_marque`),
  KEY `fk_Modele_Equipement_Protocole` (`id_protocole`),
  CONSTRAINT `fk_Modele_Equipement_Marque` FOREIGN KEY (`id_marque`) REFERENCES `marque_equipement` (`id_marque`) ON UPDATE CASCADE,
  CONSTRAINT `fk_Modele_Equipement_Protocole` FOREIGN KEY (`id_protocole`) REFERENCES `protocole` (`id_Protocole`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=35 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Les données exportées n'étaient pas sélectionnées.

-- Listage de la structure de la table tam_rpi. molecule
CREATE TABLE IF NOT EXISTS `Molecule` (
  `id_molecule` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `code` varchar(5) COLLATE latin1_general_ci NOT NULL,
  `nom` varchar(50) COLLATE latin1_general_ci NOT NULL,
  `formule` varchar(10) COLLATE latin1_general_ci NOT NULL,
  PRIMARY KEY (`id_molecule`),
  UNIQUE KEY `code_UNIQUE` (`code`),
  UNIQUE KEY `formule_UNIQUE` (`formule`)
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Les données exportées n'étaient pas sélectionnées.

-- Listage de la structure de la table tam_rpi. operateur

CREATE TABLE IF NOT EXISTS `Operateur` (
  `id_operateur` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `Nom` varchar(30) COLLATE latin1_general_ci NOT NULL,
  `Prenom` varchar(30) COLLATE latin1_general_ci NOT NULL,
  `Mot_de_passe` varchar(45) COLLATE latin1_general_ci NOT NULL DEFAULT '123',
  `Administrateur` tinyint(1) NOT NULL DEFAULT 0,
  `user_name` varchar(30) COLLATE latin1_general_ci NOT NULL,
  PRIMARY KEY (`id_operateur`),
  UNIQUE KEY `id_username` (`user_name`)
) ENGINE=InnoDB AUTO_INCREMENT=15 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Les données exportées n'étaient pas sélectionnées.

-- Listage de la structure de la table tam_rpi. polluant_associe
CREATE TABLE IF NOT EXISTS `Polluant_Associe` (
  `id_polluant_associe` int(11) NOT NULL AUTO_INCREMENT,
  `id_pa_equipement` smallint(5) unsigned NOT NULL,
  `id_pa_molecule` smallint(5) unsigned NOT NULL,
  PRIMARY KEY (`id_polluant_associe`),
  KEY `fk_Polluant_Associe_Equipement` (`id_pa_equipement`),
  KEY `fk_Polluant_Associe_Molecule` (`id_pa_molecule`),
  CONSTRAINT `fk_Polluant_Associe_Equipement` FOREIGN KEY (`id_pa_equipement`) REFERENCES `equipement` (`id_equipement`) ON UPDATE CASCADE,
  CONSTRAINT `fk_Polluant_Associe_Molecule` FOREIGN KEY (`id_pa_molecule`) REFERENCES `molecule` (`id_molecule`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=252 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Les données exportées n'étaient pas sélectionnées.

-- Listage de la structure de la table tam_rpi. port_serie
CREATE TABLE IF NOT EXISTS `Port_Serie` (
  `no_port` smallint(6) NOT NULL DEFAULT 0,
  `designation` varchar(20) COLLATE latin1_general_ci DEFAULT NULL,
  PRIMARY KEY (`no_port`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Les données exportées n'étaient pas sélectionnées.

-- Listage de la structure de la table tam_rpi. protocole
CREATE TABLE IF NOT EXISTS `Protocole` (
  `id_Protocole` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `designation` varchar(20) COLLATE latin1_general_ci NOT NULL,
  PRIMARY KEY (`id_Protocole`),
  UNIQUE KEY `designation_UNIQUE` (`designation`)
) ENGINE=InnoDB AUTO_INCREMENT=18 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Les données exportées n'étaient pas sélectionnées.

-- Listage de la structure de la table tam_rpi. systeme_etalonnage
CREATE TABLE IF NOT EXISTS `Systeme_Etalonnage` (
  `id_systeme_etalon` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `id_etalon` smallint(5) unsigned NOT NULL,
  `id_bouteille` smallint(5) unsigned NOT NULL,
  `id_gzero` smallint(5) unsigned NOT NULL,
  `canal_defaut` varchar(6) COLLATE latin1_general_ci NOT NULL,
  PRIMARY KEY (`id_systeme_etalon`)
) ENGINE=InnoDB AUTO_INCREMENT=42 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Les données exportées n'étaient pas sélectionnées.

-- Listage de la structure de la table tam_rpi. taux_transmission
CREATE TABLE IF NOT EXISTS `Taux_Transmission` (
  `id_tx_transmission` tinyint(3) unsigned NOT NULL AUTO_INCREMENT,
  `taux_transmission` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`id_tx_transmission`),
  UNIQUE KEY `taux_transmission_UNIQUE` (`taux_transmission`)
) ENGINE=InnoDB AUTO_INCREMENT=22 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Les données exportées n'étaient pas sélectionnées.

-- Listage de la structure de la table tam_rpi. test_metrologique
CREATE TABLE IF NOT EXISTS `Test_Metrologique` (
  `id_test` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `test_metro_type_test` enum('REPETABILITE','LINEARITE','TEMPS_REPONSE','RENDEMENT_FOUR','PERSO') COLLATE latin1_general_ci NOT NULL,
  `id_operateur` smallint(5) unsigned NOT NULL,
  `id_systeme_etalon` int(10) unsigned NOT NULL,
  `id_lieu` smallint(5) unsigned NOT NULL,
  `pression` int(11) DEFAULT NULL,
  `temperature` decimal(10,1) DEFAULT NULL,
  `date_debut` datetime NOT NULL,
  `date_fin` datetime NOT NULL,
  `tps_acquisition` smallint(5) NOT NULL DEFAULT 0,
  `critere1` double(6,2) NOT NULL,
  `critere2` double(6,2) NOT NULL,
  `critere3` double(6,2) NOT NULL,
  `critere_temperature_min` double(6,2) NOT NULL,
  `critere_temperature_max` double(6,2) NOT NULL,
  `critere_variation` double(6,2) NOT NULL,
  `temperature_min` decimal(10,2) DEFAULT NULL,
  `temperature_max` decimal(10,2) DEFAULT NULL,
  `temperature_moyenne` decimal(10,2) DEFAULT NULL,
  `id_sonde` smallint(5) unsigned DEFAULT NULL,
  `sondePresente` enum('OUI','NON') COLLATE latin1_general_ci DEFAULT 'NON',
  PRIMARY KEY (`id_test`),
  KEY `fk_test_operateur` (`id_operateur`),
  KEY `fk_test_lieu` (`id_lieu`),
  KEY `fk_test_system_etalon` (`id_systeme_etalon`),
  KEY `fk_id_sonde` (`id_sonde`),
  CONSTRAINT `fk_id_sonde` FOREIGN KEY (`id_sonde`) REFERENCES `equipement` (`id_equipement`)
) ENGINE=InnoDB AUTO_INCREMENT=2799 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;


-- Les données exportées n'étaient pas sélectionnées.

-- Listage de la structure de la table tam_rpi. test_xml

CREATE TABLE IF NOT EXISTS `Test_XML` (
  `id_Test_Xml` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `fichier_description` varchar(100) COLLATE latin1_general_ci NOT NULL,
  `type_test` enum('REPETABILITE','REPETABILITE_2','LINEARITE','TEMPS_REPONSE','RENDEMENT_FOUR','PERSO') COLLATE latin1_general_ci NOT NULL DEFAULT 'PERSO',
  `id_systeme_etalon` int(10) unsigned NOT NULL,
  `est_favori` enum('OUI','NON') COLLATE latin1_general_ci DEFAULT 'NON',
  PRIMARY KEY (`id_Test_Xml`),
  UNIQUE KEY `fichier_description_UNIQUE` (`fichier_description`),
  KEY `fk_Test_XML_SystemeEtalon` (`id_systeme_etalon`)
) ENGINE=InnoDB AUTO_INCREMENT=97 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Les données exportées n'étaient pas sélectionnées.

-- Listage de la structure de la table tam_rpi. validation_test

CREATE TABLE IF NOT EXISTS `Validation_Test` (
  `id_validation` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `id_test` smallint(5) unsigned NOT NULL,
  `id_analyseur` smallint(5) unsigned NOT NULL,
  `id_operateur` smallint(5) unsigned DEFAULT NULL,
  `date_validation` datetime DEFAULT NULL,
  `etat_validation` enum('VALIDE','INVALIDE','EN ATTENTE') COLLATE latin1_bin NOT NULL DEFAULT 'EN ATTENTE',
  PRIMARY KEY (`id_validation`),
  UNIQUE KEY `unique_paire_id_test_id_analyseur` (`id_test`,`id_analyseur`),
  KEY `fk_id_analyseur` (`id_analyseur`),
  KEY `fk_id_operateur` (`id_operateur`),
  CONSTRAINT `fk_id_analyseur` FOREIGN KEY (`id_analyseur`) REFERENCES `equipement` (`id_equipement`),
  CONSTRAINT `fk_id_operateur` FOREIGN KEY (`id_operateur`) REFERENCES `operateur` (`id_operateur`),
  CONSTRAINT `fk_id_test` FOREIGN KEY (`id_test`) REFERENCES `test_metrologique` (`id_test`)

) ENGINE=InnoDB AUTO_INCREMENT=22 DEFAULT CHARSET=latin1 COLLATE=latin1_bin;


-- Les données exportées n'étaient pas sélectionnées.

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
