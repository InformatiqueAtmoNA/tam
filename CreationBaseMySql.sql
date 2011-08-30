SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT=0;
START TRANSACTION;


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Base de données: `TAM_V3`
--

-- --------------------------------------------------------

--
-- Structure de la table `Concentration`
--

CREATE TABLE IF NOT EXISTS `Concentration` (
  `id_Concentration` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `id_systeme_etalon` int(10) unsigned NOT NULL,
  `id_molecule` smallint(5) unsigned NOT NULL,
  `point_consigne` smallint(6) NOT NULL,
  `conc_reelle` decimal(10,2) NOT NULL,
  `conc_ozone` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id_Concentration`),
  KEY `fk_Molecule` (`id_molecule`),
  KEY `fk_Concentration_Systeme_Etalon` (`id_systeme_etalon`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci AUTO_INCREMENT=5 ;

--
-- Contenu de la table `Concentration`
--

-- --------------------------------------------------------

--
-- Structure de la table `Concentration_Associee`
--

CREATE TABLE IF NOT EXISTS `Concentration_Associee` (
  `id_concentration_associee` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `id_concentration` int(10) unsigned NOT NULL,
  `id_molecule` smallint(5) unsigned NOT NULL,
  `concentration` decimal(10,2) unsigned NOT NULL,
  PRIMARY KEY (`id_concentration_associee`),
  KEY `fk_Concentration_Associee_Concentration` (`id_concentration`),
  KEY `fk_Concentration_Associee_Molecule` (`id_molecule`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci AUTO_INCREMENT=1 ;

--
-- Contenu de la table `Concentration_Associee`
--


-- --------------------------------------------------------

--
-- Structure de la table `Equipement`
--

CREATE TABLE IF NOT EXISTS `Equipement` (
  `id_equipement` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `id_modele` smallint(5) unsigned NOT NULL,
  `numero_serie` varchar(45) COLLATE latin1_general_ci NOT NULL,
  `en_service` tinyint(1) NOT NULL DEFAULT '0',
  `min_gamme` smallint(6) DEFAULT '0',
  `max_gamme` smallint(6) DEFAULT '0',
  `offset` smallint(6) DEFAULT '0',
  `id_tx_transmission` tinyint(3) unsigned NOT NULL,
  `adresse` varchar(10) COLLATE latin1_general_ci DEFAULT NULL,
  `nb_bits_transmission` tinyint(4) DEFAULT '0',
  `nb_bits_stop` tinyint(4) DEFAULT '0',
  `controle_flux` enum('AUCUN','HARDWARE','XON/XOFF') COLLATE latin1_general_ci DEFAULT 'AUCUN',
  `type_parite` enum('AUCUNE','ODD','EVEN','MARK','SPACE') COLLATE latin1_general_ci DEFAULT 'AUCUNE',
  `no_port` smallint(6) DEFAULT NULL,
  PRIMARY KEY (`id_equipement`),
  UNIQUE KEY `numero_serie_UNIQUE` (`numero_serie`),
  KEY `fk_eq_tx_transmission` (`id_tx_transmission`),
  KEY `fk_eq_modele` (`id_modele`),
  KEY `fk_eq_port_serie` (`no_port`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci AUTO_INCREMENT=7 ;

--
-- Contenu de la table `Equipement`
--

-- --------------------------------------------------------

--
-- Structure de la table `Equipement_Reforme`
--

CREATE TABLE IF NOT EXISTS `Equipement_Reforme` (
  `id_equipement_reforme` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `id_modele` smallint(5) unsigned NOT NULL,
  `id_equipement` smallint(5) unsigned NOT NULL,
  `date_reforme` varchar(45) COLLATE latin1_general_ci NOT NULL,
  `numero_serie` varchar(45) COLLATE latin1_general_ci NOT NULL,
  PRIMARY KEY (`id_equipement_reforme`),
  UNIQUE KEY `id_equipement_UNIQUE` (`id_equipement`),
  KEY `fk_eq_reforme_modele` (`id_modele`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci AUTO_INCREMENT=1 ;

--
-- Contenu de la table `Equipement_Reforme`
--


-- --------------------------------------------------------

--
-- Structure de la table `Lieu`
--

CREATE TABLE IF NOT EXISTS `Lieu` (
  `id_lieu` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `designation` varchar(45) COLLATE latin1_general_ci NOT NULL,
  PRIMARY KEY (`id_lieu`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci AUTO_INCREMENT=2 ;

--
-- Contenu de la table `Lieu`
--

-- --------------------------------------------------------

--
-- Structure de la table `Liste_Analyseurs_Test`
--

CREATE TABLE IF NOT EXISTS `Liste_Analyseurs_Test` (
  `id_test` smallint(5) unsigned NOT NULL,
  `id_equipement` smallint(5) unsigned NOT NULL,
  PRIMARY KEY (`id_test`,`id_equipement`),
  KEY `fk_liste_ana_test` (`id_test`),
  KEY `fk_liste_ana_equipement` (`id_equipement`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

--
-- Contenu de la table `Liste_Analyseurs_Test`
--


-- --------------------------------------------------------

--
-- Structure de la table `Marque_Equipement`
--

CREATE TABLE IF NOT EXISTS `Marque_Equipement` (
  `id_marque` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `designation` varchar(30) COLLATE latin1_general_ci NOT NULL,
  PRIMARY KEY (`id_marque`),
  UNIQUE KEY `designation_UNIQUE` (`designation`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci AUTO_INCREMENT=14 ;

--
-- Contenu de la table `Marque_Equipement`
--

INSERT INTO `Marque_Equipement` (`id_marque`, `designation`) VALUES
(2, 'AIRLIQUIDE'),
(4, 'ANSYCO'),
(5, 'API'),
(6, 'CHROMATO-SUD'),
(7, 'ENVSA'),
(8, 'HORIBA'),
(1, 'INCONNUE'),
(9, 'LNI'),
(10, 'MESSER'),
(11, 'SERES'),
(12, 'SYNSPEC'),
(13, 'TEI');

-- --------------------------------------------------------

--
-- Structure de la table `Mesure`
--

CREATE TABLE IF NOT EXISTS `Mesure` (
  `id_mesure` smallint(5) unsigned NOT NULL,
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
  KEY `fk_mesure_equipement` (`id_equipement`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

--
-- Contenu de la table `Mesure`
--


-- --------------------------------------------------------

--
-- Structure de la table `Modele_Equipement`
--

CREATE TABLE IF NOT EXISTS `Modele_Equipement` (
  `id_modele` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `id_marque` smallint(5) unsigned NOT NULL,
  `id_protocole` smallint(5) unsigned NOT NULL,
  `designation` varchar(45) COLLATE latin1_general_ci DEFAULT NULL,
  `type` enum('ANALYSEUR','DILUTEUR','BOUTEILLE','GO3','GZERO') COLLATE latin1_general_ci DEFAULT NULL,
  PRIMARY KEY (`id_modele`),
  KEY `fk_Modele_Equipement_Marque` (`id_marque`),
  KEY `fk_Modele_Equipement_Protocole` (`id_protocole`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci AUTO_INCREMENT=8 ;

--
-- Contenu de la table `Modele_Equipement`
--

INSERT INTO `Modele_Equipement` (`id_modele`, `id_marque`, `id_protocole`, `designation`, `type`) VALUES
(1, 1, 1, 'INCONNU', 'BOUTEILLE'),
(2, 1, 1, 'INCONNU', 'GZERO'),

-- --------------------------------------------------------

--
-- Structure de la table `Molecule`
--

CREATE TABLE IF NOT EXISTS `Molecule` (
  `id_molecule` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `code` varchar(5) COLLATE latin1_general_ci NOT NULL,
  `nom` varchar(50) COLLATE latin1_general_ci NOT NULL,
  `formule` varchar(10) COLLATE latin1_general_ci NOT NULL,
  PRIMARY KEY (`id_molecule`),
  UNIQUE KEY `code_UNIQUE` (`code`),
  UNIQUE KEY `formule_UNIQUE` (`formule`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci AUTO_INCREMENT=7 ;

--
-- Contenu de la table `Molecule`
--

INSERT INTO `Molecule` (`id_molecule`, `code`, `nom`, `formule`) VALUES
(1, '1', 'Dioxyde de soufre', 'SO2'),
(2, '2', 'Monoxyde d''azote', 'NO'),
(3, '3', 'Dioxyde d''azote', 'NO2'),
(4, '4', 'Monoxyde de carbone', 'CO'),
(5, '8', 'Ozone', 'O3'),
(6, '12', 'Oxydes d''azote', 'NOX');

-- --------------------------------------------------------

--
-- Structure de la table `Operateur`
--

CREATE TABLE IF NOT EXISTS `Operateur` (
  `id_operateur` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `Nom` varchar(30) COLLATE latin1_general_ci NOT NULL,
  `Prenom` varchar(30) COLLATE latin1_general_ci NOT NULL,
  `Mot_de_passe` varchar(45) COLLATE latin1_general_ci DEFAULT NULL,
  `Administrateur` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`id_operateur`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci AUTO_INCREMENT=2 ;

--
-- Contenu de la table `Operateur`
--

-- --------------------------------------------------------

--
-- Structure de la table `Polluant_Associe`
--

CREATE TABLE IF NOT EXISTS `Polluant_Associe` (
  `id_polluant_associe` int(11) NOT NULL AUTO_INCREMENT,
  `id_pa_equipement` smallint(5) unsigned NOT NULL,
  `id_pa_molecule` smallint(5) unsigned NOT NULL,
  PRIMARY KEY (`id_polluant_associe`),
  KEY `fk_Polluant_Associe_Equipement` (`id_pa_equipement`),
  KEY `fk_Polluant_Associe_Molecule` (`id_pa_molecule`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci AUTO_INCREMENT=19 ;

--
-- Contenu de la table `Polluant_Associe`
--

-- --------------------------------------------------------

--
-- Structure de la table `Port_Serie`
--

CREATE TABLE IF NOT EXISTS `Port_Serie` (
  `no_port` smallint(6) NOT NULL DEFAULT '0',
  `designation` varchar(20) COLLATE latin1_general_ci DEFAULT NULL,
  PRIMARY KEY (`no_port`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

--
-- Contenu de la table `Port_Serie`
--


-- --------------------------------------------------------

--
-- Structure de la table `Protocole`
--

CREATE TABLE IF NOT EXISTS `Protocole` (
  `id_Protocole` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `designation` varchar(20) COLLATE latin1_general_ci NOT NULL,
  PRIMARY KEY (`id_Protocole`),
  UNIQUE KEY `designation_UNIQUE` (`designation`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci AUTO_INCREMENT=14 ;

--
-- Contenu de la table `Protocole`
--

INSERT INTO `Protocole` (`id_Protocole`, `designation`) VALUES
(10, 'API_ANA'),
(11, 'API_DIL'),
(1, 'INCONNU'),
(2, 'MODE4_ANA_CMD04'),
(3, 'MODE4_ANA_CMD16'),
(5, 'MODE4_LNI'),
(4, 'MODE4_SX6000_17'),
(7, 'TEI_146c'),
(8, 'TEI_146i'),
(13, 'TEI_146i_OZONISEUR'),
(12, 'TEI_146i_PHOTOMETRE'),
(9, 'TEI_49ps'),
(6, 'TEI_ANA');

-- --------------------------------------------------------

--
-- Structure de la table `Systeme_Etalonnage`
--

CREATE TABLE IF NOT EXISTS `Systeme_Etalonnage` (
  `id_systeme_etalon` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `id_etalon` smallint(5) unsigned NOT NULL,
  `id_bouteille` smallint(5) unsigned NOT NULL,
  `id_gzero` smallint(5) unsigned NOT NULL,
  PRIMARY KEY (`id_systeme_etalon`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci AUTO_INCREMENT=65 ;

--
-- Contenu de la table `Systeme_Etalonnage`
--

-- --------------------------------------------------------

--
-- Structure de la table `Taux_Transmission`
--

CREATE TABLE IF NOT EXISTS `Taux_Transmission` (
  `id_tx_transmission` tinyint(3) unsigned NOT NULL AUTO_INCREMENT,
  `taux_transmission` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`id_tx_transmission`),
  UNIQUE KEY `taux_transmission_UNIQUE` (`taux_transmission`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci AUTO_INCREMENT=22 ;

--
-- Contenu de la table `Taux_Transmission`
--

INSERT INTO `Taux_Transmission` (`id_tx_transmission`, `taux_transmission`) VALUES
(1, 50),
(2, 75),
(3, 110),
(4, 134),
(5, 150),
(6, 200),
(7, 300),
(8, 600),
(9, 1200),
(10, 1800),
(11, 2400),
(12, 4800),
(13, 9600),
(14, 14400),
(15, 19200),
(16, 38400),
(17, 56000),
(18, 57600),
(19, 76800),
(20, 115200),
(21, 128000);

-- --------------------------------------------------------

--
-- Structure de la table `Test_Metrologique`
--

CREATE TABLE IF NOT EXISTS `Test_Metrologique` (
  `id_test` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `id_description_xml` smallint(5) unsigned NOT NULL,
  `id_operateur` smallint(5) unsigned NOT NULL,
  `id_lieu` smallint(5) unsigned NOT NULL,
  `pression` int(11) DEFAULT NULL,
  `temperature` decimal(10,0) DEFAULT NULL,
  `date_debut` datetime NOT NULL,
  `date_fin` datetime NOT NULL,
  PRIMARY KEY (`id_test`),
  KEY `fk_test_xml` (`id_description_xml`),
  KEY `fk_test_operateur` (`id_operateur`),
  KEY `fk_test_lieu` (`id_lieu`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci AUTO_INCREMENT=1 ;

--
-- Contenu de la table `Test_Metrologique`
--


-- --------------------------------------------------------

--
-- Structure de la table `Test_XML`
--

CREATE TABLE IF NOT EXISTS `Test_XML` (
  `id_Test_Xml` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `fichier_description` varchar(45) COLLATE latin1_general_ci NOT NULL,
  `type_test` enum('REPETABILITE_1','REPETABILITE_2','LINEARITE','TEMPS_REPONSE','RENDEMENT_FOUR','PERSO') COLLATE latin1_general_ci NOT NULL DEFAULT 'PERSO',
  `id_systeme_etalon` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id_Test_Xml`),
  UNIQUE KEY `fichier_description_UNIQUE` (`fichier_description`),
  KEY `fk_Test_XML_SystemeEtalon` (`id_systeme_etalon`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci AUTO_INCREMENT=5 ;

--
-- Contenu de la table `Test_XML`
--

--
-- Contraintes pour les tables exportées
--

--
-- Contraintes pour la table `Concentration`
--
ALTER TABLE `Concentration`
  ADD CONSTRAINT `fk_Molecule` FOREIGN KEY (`id_molecule`) REFERENCES `Molecule` (`id_molecule`) ON UPDATE CASCADE,
  ADD CONSTRAINT `fk_Concentration_Systeme_Etalon` FOREIGN KEY (`id_systeme_etalon`) REFERENCES `Systeme_Etalonnage` (`id_systeme_etalon`) ON UPDATE CASCADE;

--
-- Contraintes pour la table `Concentration_Associee`
--
ALTER TABLE `Concentration_Associee`
  ADD CONSTRAINT `fk_Concentration_Associee_Concentration` FOREIGN KEY (`id_concentration`) REFERENCES `Concentration` (`id_Concentration`) ON UPDATE CASCADE,
  ADD CONSTRAINT `fk_Concentration_Associee_Molecule` FOREIGN KEY (`id_molecule`) REFERENCES `Molecule` (`id_molecule`) ON UPDATE CASCADE;

--
-- Contraintes pour la table `Equipement`
--
ALTER TABLE `Equipement`
  ADD CONSTRAINT `fk_eq_tx_transmission` FOREIGN KEY (`id_tx_transmission`) REFERENCES `Taux_Transmission` (`id_tx_transmission`) ON UPDATE CASCADE,
  ADD CONSTRAINT `fk_eq_modele` FOREIGN KEY (`id_modele`) REFERENCES `Modele_Equipement` (`id_modele`) ON UPDATE CASCADE,
  ADD CONSTRAINT `fk_eq_port_serie` FOREIGN KEY (`no_port`) REFERENCES `Port_Serie` (`no_port`) ON UPDATE CASCADE;

--
-- Contraintes pour la table `Equipement_Reforme`
--
ALTER TABLE `Equipement_Reforme`
  ADD CONSTRAINT `fk_eq_reforme_modele` FOREIGN KEY (`id_modele`) REFERENCES `Modele_Equipement` (`id_modele`) ON UPDATE CASCADE;

--
-- Contraintes pour la table `Liste_Analyseurs_Test`
--
ALTER TABLE `Liste_Analyseurs_Test`
  ADD CONSTRAINT `fk_liste_ana_test` FOREIGN KEY (`id_test`) REFERENCES `Test_Metrologique` (`id_test`) ON UPDATE CASCADE,
  ADD CONSTRAINT `fk_liste_ana_equipement` FOREIGN KEY (`id_equipement`) REFERENCES `Equipement` (`id_equipement`) ON UPDATE CASCADE;

--
-- Contraintes pour la table `Mesure`
--
ALTER TABLE `Mesure`
  ADD CONSTRAINT `fk_mesure_equipement` FOREIGN KEY (`id_equipement`) REFERENCES `Equipement` (`id_equipement`) ON UPDATE CASCADE,
  ADD CONSTRAINT `fk_mesure_test` FOREIGN KEY (`id_test`) REFERENCES `Test_Metrologique` (`id_test`) ON UPDATE CASCADE;

--
-- Contraintes pour la table `Modele_Equipement`
--
ALTER TABLE `Modele_Equipement`
  ADD CONSTRAINT `fk_Modele_Equipement_Marque` FOREIGN KEY (`id_marque`) REFERENCES `Marque_Equipement` (`id_marque`) ON UPDATE CASCADE,
  ADD CONSTRAINT `fk_Modele_Equipement_Protocole` FOREIGN KEY (`id_protocole`) REFERENCES `Protocole` (`id_Protocole`) ON UPDATE CASCADE;

--
-- Contraintes pour la table `Polluant_Associe`
--
ALTER TABLE `Polluant_Associe`
  ADD CONSTRAINT `fk_Polluant_Associe_Equipement` FOREIGN KEY (`id_pa_equipement`) REFERENCES `Equipement` (`id_equipement`) ON UPDATE CASCADE,
  ADD CONSTRAINT `fk_Polluant_Associe_Molecule` FOREIGN KEY (`id_pa_molecule`) REFERENCES `Molecule` (`id_molecule`) ON UPDATE CASCADE;

--
-- Contraintes pour la table `Test_Metrologique`
--
ALTER TABLE `Test_Metrologique`
  ADD CONSTRAINT `fk_test_lieu` FOREIGN KEY (`id_lieu`) REFERENCES `Lieu` (`id_lieu`) ON UPDATE CASCADE,
  ADD CONSTRAINT `fk_test_operateur` FOREIGN KEY (`id_operateur`) REFERENCES `Operateur` (`id_operateur`) ON UPDATE CASCADE,
  ADD CONSTRAINT `fk_test_xml` FOREIGN KEY (`id_description_xml`) REFERENCES `Test_XML` (`id_Test_Xml`) ON UPDATE CASCADE;

--
-- Contraintes pour la table `Test_XML`
--
ALTER TABLE `Test_XML`
  ADD CONSTRAINT `fk_Test_XML_SystemeEtalon` FOREIGN KEY (`id_systeme_etalon`) REFERENCES `Systeme_Etalonnage` (`id_systeme_etalon`) ON UPDATE CASCADE;
COMMIT;
