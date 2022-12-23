#ifndef SIGNATURE_HPP
#define SIGNATURE_HPP

#include <vector>

namespace Kuma3D {

// A Signature is used to identify what components are attached to an Entity.
using Signature = std::vector<char>;

/**
 * Returns whether a Signature is relevant to another Signature (whether
 * it is contained in the Signature).
 *
 * @param aSignature The Signature to check relevancy of.
 * @param aBaseSignature The Signature that may contain another Signature.
 */
inline bool IsSignatureRelevant(const Signature& aSignature,
                                const Signature& aBaseSignature)
{
  bool success = true;

  for(int i = 0; i < aBaseSignature.size(); ++i)
  {
    if(aBaseSignature[i] && !aSignature[i])
    {
      success = false;
      break;
    }
  }

  return success;
};

} // namespace Kuma3D

#endif
