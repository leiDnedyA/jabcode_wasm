import wasmBindings from './build/libjabcode.js';

const FS = wasmBindings.FS;
const filename = 'image.png';

async function jpegBlobToPngBlob(jpegBlob) {
  return new Promise((resolve, reject) => {
    const img = new Image();
    const reader = new FileReader();

    reader.onerror = reject;
    img.onerror = reject;

    reader.onload = () => {
      img.src = reader.result;
    };

    img.onload = async () => {
      const canvas = document.createElement('canvas');
      canvas.width = img.width;
      canvas.height = img.height;

      const ctx = canvas.getContext('2d');
      ctx.drawImage(img, 0, 0);
      canvas.toBlob(
        blob => {
          if (blob) resolve(blob);
          else reject(new Error('Canvas toBlob returned null'));
        },
        'image/png'
      );
    };
    reader.readAsDataURL(jpegBlob);
  });
}

const jabcode = {
  readImage: async (rawImageBlob) => {
    const isJpeg = rawImageBlob.type === 'image/jpeg';
    const imageBlob = isJpeg ? await jpegBlobToPngBlob(rawImageBlob) : rawImageBlob;
    if (!FS.analyzePath('/working').exists) {
      FS.mkdir('/working');
    }
    const filepath = `/working/${filename}`;
    const arrayBuffer = await imageBlob.arrayBuffer();
    FS.writeFile(filepath, new Uint8Array(arrayBuffer), { encoding: 'binary' });

    const pathPtr = wasmBindings.stringToNewUTF8(filepath);
    let resultPtr = null;
    let resultString = null;
    try {
      resultPtr = wasmBindings._read_image(pathPtr);
      resultString = wasmBindings.UTF8ToString(resultPtr);
    } finally {
      wasmBindings._free(pathPtr);
      wasmBindings._free(resultPtr);
    }
    return resultString;
  }
};

export default jabcode;
