/*
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER.
 * 
 * Copyright (c) 2010-2011 Oracle and/or its affiliates. All rights reserved.
 * 
 * The contents of this file are subject to the terms of either the GNU General Public License Version 2 only ("GPL") or the Common
 * Development and Distribution License("CDDL") (collectively, the "License"). You may not use this file except in compliance with the
 * License. You can obtain a copy of the License at http://glassfish.java.net/public/CDDL+GPL_1_1.html or packager/legal/LICENSE.txt. See
 * the License for the specific language governing permissions and limitations under the License.
 * 
 * When distributing the software, include this License Header Notice in each file and include the License file at
 * packager/legal/LICENSE.txt.
 * 
 * GPL Classpath Exception: Oracle designates this particular file as subject to the "Classpath" exception as provided by Oracle in the GPL
 * Version 2 section of the License file that accompanied this code.
 * 
 * Modifications: If applicable, add the following below the License Header, with the fields enclosed by brackets [] replaced by your own
 * identifying information: "Portions Copyright [year] [name of copyright owner]"
 * 
 * Contributor(s): If you wish your version of this file to be governed by only the CDDL or only the GPL Version 2, indicate your decision
 * by adding "[Contributor] elects to include this software in this distribution under the [CDDL or GPL Version 2] license." If you don't
 * indicate a single choice of license, a recipient has the option to distribute your version of this file under either the CDDL, the GPL
 * Version 2 or to extend the choice of license to its licensees as provided above. However, if you add GPL Version 2 code and therefore,
 * elected the GPL Version 2 license, then the option applies only if the new code is made subject to such option by the copyright holder.
 */

package eu.imageau.monitorSMD.WebService;

import java.io.IOException;

import org.glassfish.grizzly.http.server.HttpServer;

import com.sun.jersey.api.container.grizzly2.GrizzlyServerFactory;
import com.sun.jersey.api.core.PackagesResourceConfig;
import com.sun.jersey.api.core.ResourceConfig;

public class Main
{
   public static void main (String[] args) throws IOException
   {
      try {
         java.util.logging.LogManager.getLogManager ().readConfiguration (
               Main.class.getClassLoader ().getResourceAsStream ("logging.properties"));
      }
      catch (SecurityException e) {
         // TODO Auto-generated catch block
         e.printStackTrace ();
      }
      catch (IOException e) {
         // TODO Auto-generated catch block
         e.printStackTrace ();
      }

      ResourceConfig rc = new PackagesResourceConfig ("eu.imageau.monitorSMD.WebService.resources");
      HttpServer httpServer = GrizzlyServerFactory.createHttpServer ("http://0.0.0.0:9998/", rc);
      System.out.println ("L'application de WebService est sur le Port http://localhost:9998/");
      System.in.read ();
      httpServer.stop ();
   }
}
