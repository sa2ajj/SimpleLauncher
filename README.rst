====================================================
Simple Launcher -- simply launches your applications
====================================================

.. contents::

News
====

18 Sep, 2007: 0.9.5 is released:
    * applied a patch to show informational banner when the application is being started (thanks
      to Marko Nykanen)
    * do not hard code configuration file name, use home directory instead (should help those
      who try to run the applet in scratchbox)

15 May, 2007: 0.9.4 is released:
    New stuff:
        * option to make background transparent
        * option to change icon size

28 March, 2007: 0.9.3 is released:
    New stuff:
        * config file can now be backed up
        * default icon is shown
        * non d-bus apps can be started

    Fixes:
        * minimum size for the applet is just 1 button :)

23 March, 2007: 0.9.1 is released:
    * fixed minimum size problem


22 March, 2007: 0.9 is released:
    * first public release

What this is?
=============

Simple launcher is an home applet for `Maemo <http://maemo.org>`_ based systems.

By default you will have to navigate to your application through the menu system Hildon Desktop
offers.  However there is certainly a number of applications that you run most often.  This small
applet allows you to run your favourite applications with just one click (one click seems to be
quite popular these days :))

Install
=======

You can check the `'Files' section <https://garage.maemo.org/frs/?group_id=131>`_ at garage.  All
releases should be available there.

The current release -- 0.9.5 -- is available for `direct download
<https://garage.maemo.org/frs/download.php/2195/simple-launcher_0.9.5_armel.deb>`_.

Configure
=========

After you installed the applet, you need to open home application menu, click on ``Select
applets...``, click on checkbox next to ``Simple Launcher``, and press ``OK``. You will be put into
``Edit Layout`` mode where you can select the location and the size of the applet. After you are
happy with with the settings, click on green checkbox button at the top right corner. Now you can
configure the applet: open the menu, select ``Applet settings``, and choose ``Launcher
settings...``.  In the dialog you should now see, you can select the applications you want to use as
well as order them according to your preferences. When you are done, press ``OK``.

.. note::

  if you upgrade the applet, you must re-boot the device, otherwise the old version
  will be used by maemo-af-desktop/hildon-desktop :)

Screenshots
===========

..
    <img src="http://pics.livejournal.com/lvader/pic/0000s9x9" border="0"/>

Bug Reports/Feature Requests
============================

Please use the `tracker <https://garage.maemo.org/tracker/?group_id=131>`_ at `garage
<https://garage.maemo.org>`_.

Subversion Repository Access
============================

You can find instructions at `SCM page @ garage <https://garage.maemo.org/scm/?group_id=131>`_ or
you can simply `browse <https://garage.maemo.org/plugins/scmsvn/viewcvs.php/?root=simple-launcher>`_
the svn repository.

..
  vim:tw=100:et
