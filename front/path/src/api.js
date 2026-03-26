export async function generateMaze({ width, height, start, end, baseUrl = 'http://127.0.0.1:18080' }) {
  const res = await fetch(`${baseUrl}/api/maze`, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({
      width,
      height,
      start,
      end
    })
  });

  if (!res.ok) {
    const text = await res.text().catch(() => '');
    throw new Error(`Maze generation failed (${res.status}): ${text}`);
  }
  const data = await res.json();
  // Expecting: { status, width, height, obstacles: [{x, y}, ...] }
  if (!data || !Array.isArray(data.obstacles)) {
    throw new Error('Invalid maze response shape');
  }
  return data.obstacles;
}
