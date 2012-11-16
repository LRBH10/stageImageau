/* $Id$ */
package eu.imageau.common.dbTools;

import java.io.Serializable;

/**
 * Force to have all persistent object to have a getId function
 * @author bdm
 */
public interface PersistentObject extends Serializable
{
   public Serializable getId ();
}
