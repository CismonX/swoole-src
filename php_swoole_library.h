/**
 * Generated by build-library.php, Please DO NOT modify!
 */

static const char* swoole_library_source_constants =
    "\n"
    "define('SWOOLE_LIBRARY', true);\n";

static const char* swoole_library_source_std_array =
    "\n"
    "function swoole_array_walk(array &$array, callable $callback, ...$userdata): bool\n"
    "{\n"
    "    if (($argc = func_num_args()) > 3) {\n"
    "        throw new TypeError(\"array_walk() expects at most 3 parameters, {$argc} given\");\n"
    "    }\n"
    "    foreach ($array as $key => &$item) {\n"
    "        $callback($item, $key, ...$userdata);\n"
    "    }\n"
    "    return true;\n"
    "}\n"
    "\n"
    "function swoole_array_walk_recursive(array &$array, callable $callback, ...$userdata): bool\n"
    "{\n"
    "    if (($argc = func_num_args()) > 3) {\n"
    "        throw new TypeError(\"array_walk_recursive() expects at most 3 parameters, {$argc} given\");\n"
    "    }\n"
    "    foreach ($array as $key => &$item) {\n"
    "        if (is_array($item)) {\n"
    "            swoole_array_walk_recursive($item, $callback, ...$userdata);\n"
    "        } else {\n"
    "            $callback($item, $key, ...$userdata);\n"
    "        }\n"
    "    }\n"
    "    return true;\n"
    "}\n";

static const char* swoole_library_source_ext_curl =
    "\n"
    "\n"
    "class swoole_curl_handler\n"
    "{\n"
    "    /** @var Swoole\\Coroutine\\Http\\Client */\n"
    "    private $client;\n"
    "    private $info;\n"
    "    private $outputStream;\n"
    "\n"
    "    /** @var callable */\n"
    "    private $headerFunction;\n"
    "    /** @var callable */\n"
    "    private $readFunction;\n"
    "    /** @var callable */\n"
    "    private $writeFunction;\n"
    "    /** @var callable */\n"
    "    private $progressFunction;\n"
    "\n"
    "    public $returnTransfer = true;\n"
    "    public $method = 'GET';\n"
    "    public $headers = [];\n"
    "\n"
    "    function create(string $url)\n"
    "    {\n"
    "        $info = parse_url($url);\n"
    "        $ssl = $info['scheme'] === 'https';\n"
    "        if (empty($info['port'])) {\n"
    "            $port = $ssl ? 443 : 80;\n"
    "        } else {\n"
    "            $port = intval($info['port']);\n"
    "        }\n"
    "        $this->info = $info;\n"
    "        $this->client = new Swoole\\Coroutine\\Http\\Client($info['host'], $port, $ssl);\n"
    "    }\n"
    "\n"
    "    function execute()\n"
    "    {\n"
    "        $client = $this->client;\n"
    "        $client->setMethod($this->method);\n"
    "        if ($this->headers) {\n"
    "            $client->setHeaders($this->headers);\n"
    "        }\n"
    "        if (!$client->execute($this->getUrl())) {\n"
    "            return false;\n"
    "        }\n"
    "\n"
    "        if ($client->headers and $this->headerFunction) {\n"
    "            $cb = $this->headerFunction;\n"
    "            if ($client->statusCode === 200) {\n"
    "                $cb($this, \"HTTP/1.1 200 OK\\r\\n\");\n"
    "            }\n"
    "            foreach ($client->headers as $k => $v) {\n"
    "                $cb($this, \"$k: $v\\r\\n\");\n"
    "            }\n"
    "            $cb($this, '');\n"
    "        }\n"
    "\n"
    "        if ($client->body and $this->readFunction) {\n"
    "            $cb = $this->readFunction;\n"
    "            $cb($this, $this->outputStream, strlen($client->body));\n"
    "        }\n"
    "\n"
    "        if ($this->returnTransfer) {\n"
    "            return $client->body;\n"
    "        } else {\n"
    "            if ($this->outputStream) {\n"
    "                return fwrite($this->outputStream, $client->body) === strlen($client->body);\n"
    "            } else {\n"
    "                echo $this->outputStream;\n"
    "            }\n"
    "            return true;\n"
    "        }\n"
    "    }\n"
    "\n"
    "    function close(): void\n"
    "    {\n"
    "        $this->client = null;\n"
    "    }\n"
    "\n"
    "    function getErrorCode(): int\n"
    "    {\n"
    "        return $this->client->errCode;\n"
    "    }\n"
    "\n"
    "    function getErrorMsg(): string\n"
    "    {\n"
    "        return $this->client->errMsg;\n"
    "    }\n"
    "\n"
    "    private function getUrl(): string\n"
    "    {\n"
    "        if (empty($this->info['path'])) {\n"
    "            $url = '/';\n"
    "        } else {\n"
    "            $url = $this->info['path'];\n"
    "        }\n"
    "        if (!empty($this->info['query'])) {\n"
    "            $url .= '?' . $this->info['query'];\n"
    "        }\n"
    "        if (!empty($this->info['query'])) {\n"
    "            $url .= '#' . $this->info['fragment'];\n"
    "        }\n"
    "        return $url;\n"
    "    }\n"
    "\n"
    "    function setOption(int $opt, $value): bool\n"
    "    {\n"
    "        switch ($opt) {\n"
    "            case CURLOPT_URL:\n"
    "                $this->create($value);\n"
    "                break;\n"
    "            case CURLOPT_RETURNTRANSFER:\n"
    "                $this->returnTransfer = $value;\n"
    "                break;\n"
    "            case CURLOPT_ENCODING:\n"
    "                if (empty($value)) {\n"
    "                    $value = 'gzip';\n"
    "                }\n"
    "                $this->headers['Accept-Encoding'] = $value;\n"
    "                break;\n"
    "            case CURLOPT_POST:\n"
    "                $this->method = 'POST';\n"
    "                break;\n"
    "            case CURLOPT_HTTPHEADER:\n"
    "                foreach ($value as $header) {\n"
    "                    list($k, $v) = explode(':', $header);\n"
    "                    $v = trim($v);\n"
    "                    if ($v) {\n"
    "                        $this->headers[$k] = $v;\n"
    "                    }\n"
    "                }\n"
    "                break;\n"
    "            case CURLOPT_CUSTOMREQUEST:\n"
    "                break;\n"
    "            case CURLOPT_PROTOCOLS:\n"
    "                if ($value > 3) {\n"
    "                    throw new swoole_curl_exception(\"option[{$opt}={$value}] not supported\");\n"
    "                }\n"
    "                break;\n"
    "            case CURLOPT_HTTP_VERSION:\n"
    "                break;\n"
    "            case CURLOPT_SSL_VERIFYHOST:\n"
    "                break;\n"
    "            case CURLOPT_SSL_VERIFYPEER:\n"
    "                $this->client->set(['ssl_verify_peer' => $value]);\n"
    "                break;\n"
    "            case CURLOPT_CONNECTTIMEOUT:\n"
    "                $this->client->set(['connect_timeout' => $value]);\n"
    "                break;\n"
    "            case CURLOPT_FILE:\n"
    "                $this->outputStream = $value;\n"
    "                break;\n"
    "            case CURLOPT_HEADER:\n"
    "                break;\n"
    "            case CURLOPT_HEADERFUNCTION:\n"
    "                $this->headerFunction = $value;\n"
    "                break;\n"
    "            case CURLOPT_READFUNCTION:\n"
    "                $this->readFunction = $value;\n"
    "                break;\n"
    "            case CURLOPT_WRITEFUNCTION:\n"
    "                $this->writeFunction = $value;\n"
    "                break;\n"
    "            case CURLOPT_PROGRESSFUNCTION:\n"
    "                $this->progressFunction = $value;\n"
    "                break;\n"
    "            default:\n"
    "                throw new swoole_curl_exception(\"option[{$opt}] not supported\");\n"
    "        }\n"
    "        return true;\n"
    "    }\n"
    "\n"
    "    function reset(): void\n"
    "    {\n"
    "        $this->client->body = '';\n"
    "    }\n"
    "}\n"
    "\n"
    "class swoole_curl_exception extends swoole_exception\n"
    "{\n"
    "\n"
    "}\n"
    "\n"
    "function swoole_curl_init(): swoole_curl_handler\n"
    "{\n"
    "    return new swoole_curl_handler();\n"
    "}\n"
    "\n"
    "function swoole_curl_setopt(swoole_curl_handler $obj, $opt, $value): bool\n"
    "{\n"
    "    return $obj->setOption($opt, $value);\n"
    "}\n"
    "\n"
    "function swoole_curl_setopt_array(swoole_curl_handler $obj, $array): bool\n"
    "{\n"
    "    foreach ($array as $k => $v) {\n"
    "        if ($obj->setOption($k, $v) === false) {\n"
    "            return false;\n"
    "        }\n"
    "    }\n"
    "    return true;\n"
    "}\n"
    "\n"
    "function swoole_curl_exec(swoole_curl_handler $obj)\n"
    "{\n"
    "    return $obj->execute();\n"
    "}\n"
    "\n"
    "function swoole_curl_close(swoole_curl_handler $obj): void\n"
    "{\n"
    "    $obj->close();\n"
    "}\n"
    "\n"
    "function swoole_curl_error(swoole_curl_handler $obj): string\n"
    "{\n"
    "    return $obj->getErrorMsg();\n"
    "}\n"
    "\n"
    "function swoole_curl_errno(swoole_curl_handler $obj): int\n"
    "{\n"
    "    return $obj->getErrorCode();\n"
    "}\n"
    "\n"
    "function swoole_curl_reset(swoole_curl_handler $obj): void\n"
    "{\n"
    "    $obj->reset();\n"
    "}\n";

static const char* swoole_library_source_core_coroutine_wait_group =
    "\n"
    "\n"
    "namespace Swoole\\Coroutine;\n"
    "\n"
    "use BadMethodCallException;\n"
    "use InvalidArgumentException;\n"
    "\n"
    "class WaitGroup\n"
    "{\n"
    "    protected $chan;\n"
    "    protected $count = 0;\n"
    "    protected $waiting = false;\n"
    "\n"
    "    public function __construct()\n"
    "    {\n"
    "        $this->chan = new Channel(1);\n"
    "    }\n"
    "\n"
    "    public function add(int $delta = 1): void\n"
    "    {\n"
    "        if ($this->waiting) {\n"
    "            throw new BadMethodCallException('WaitGroup misuse: add called concurrently with wait');\n"
    "        }\n"
    "        $count = $this->count + $delta;\n"
    "        if ($count < 0) {\n"
    "            throw new InvalidArgumentException('negative WaitGroup counter');\n"
    "        }\n"
    "        $this->count = $count;\n"
    "    }\n"
    "\n"
    "    public function done(): void\n"
    "    {\n"
    "        $count = $this->count - 1;\n"
    "        if ($count < 0) {\n"
    "            throw new BadMethodCallException('negative WaitGroup counter');\n"
    "        }\n"
    "        $this->count = $count;\n"
    "        if ($count === 0 && $this->waiting) {\n"
    "            $this->chan->push(true);\n"
    "        }\n"
    "    }\n"
    "\n"
    "    public function wait(): void\n"
    "    {\n"
    "        if ($this->count > 0) {\n"
    "            $this->waiting = true;\n"
    "            $this->chan->pop();\n"
    "            $this->waiting = false;\n"
    "        }\n"
    "    }\n"
    "}\n";

static const char* swoole_library_source_core_string_object =
    "\n"
    "\n"
    "namespace Swoole;\n"
    "\n"
    "class StringObject\n"
    "{\n"
    "    /**\n"
    "     * @var string\n"
    "     */\n"
    "    protected $string;\n"
    "\n"
    "    /**\n"
    "     * StringObject constructor.\n"
    "     * @param $string\n"
    "     */\n"
    "    public function __construct(string $string = '')\n"
    "    {\n"
    "        $this->string = $string;\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @return int\n"
    "     */\n"
    "    public function length(): int\n"
    "    {\n"
    "        return strlen($this->string);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param string $needle\n"
    "     * @param int $offset\n"
    "     * @return bool|int\n"
    "     */\n"
    "    public function indexOf(string $needle, int $offset = 0)\n"
    "    {\n"
    "        return strpos($this->string, $needle, $offset);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param string $needle\n"
    "     * @param int $offset\n"
    "     * @return bool|int\n"
    "     */\n"
    "    public function lastIndexOf(string $needle, int $offset = 0)\n"
    "    {\n"
    "        return strrpos($this->string, $needle, $offset);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param string $needle\n"
    "     * @param int $offset\n"
    "     * @return bool|int\n"
    "     */\n"
    "    public function pos(string $needle, int $offset = 0)\n"
    "    {\n"
    "        return strpos($this->string, $needle, $offset);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param string $needle\n"
    "     * @param int $offset\n"
    "     * @return bool|int\n"
    "     */\n"
    "    public function rpos(string $needle, int $offset = 0)\n"
    "    {\n"
    "        return strrpos($this->string, $needle, $offset);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param string $needle\n"
    "     * @return bool|int\n"
    "     */\n"
    "    public function ipos(string $needle)\n"
    "    {\n"
    "        return stripos($this->string, $needle);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @return static\n"
    "     */\n"
    "    public function lower(): self\n"
    "    {\n"
    "        return new static(strtolower($this->string));\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @return static\n"
    "     */\n"
    "    public function upper(): self\n"
    "    {\n"
    "        return new static(strtoupper($this->string));\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @return static\n"
    "     */\n"
    "    public function trim(): self\n"
    "    {\n"
    "        return new static(trim($this->string));\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @return static\n"
    "     */\n"
    "    public function lrim(): self\n"
    "    {\n"
    "        return new static(ltrim($this->string));\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @return static\n"
    "     */\n"
    "    public function rtrim(): self\n"
    "    {\n"
    "        return new static(rtrim($this->string));\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param int $offset\n"
    "     * @param mixed ...$length\n"
    "     * @return StringObject\n"
    "     */\n"
    "    public function substr(int $offset, ...$length): self\n"
    "    {\n"
    "        return new static(substr($this->string, $offset, ...$length));\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param string $search\n"
    "     * @param string $replace\n"
    "     * @param null $count\n"
    "     * @return static\n"
    "     */\n"
    "    public function replace(string $search, string $replace, &$count = null): self\n"
    "    {\n"
    "        return new static(str_replace($search, $replace, $this->string, $count));\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param string $needle\n"
    "     * @return bool\n"
    "     */\n"
    "    public function startsWith(string $needle): bool\n"
    "    {\n"
    "        return $this->pos($needle) === 0;\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param string $subString\n"
    "     * @return bool\n"
    "     */\n"
    "    public function contains(string $subString): bool\n"
    "    {\n"
    "        return $this->pos($subString) !== false;\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param string $needle\n"
    "     * @return bool\n"
    "     */\n"
    "    public function endsWith(string $needle): bool\n"
    "    {\n"
    "        $length = strlen($needle);\n"
    "        if ($length == 0) {\n"
    "            return true;\n"
    "        }\n"
    "        return substr($this->string, -$length) === $needle;\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param string $delimiter\n"
    "     * @param int $limit\n"
    "     * @return ArrayObject\n"
    "     */\n"
    "    public function split(string $delimiter, int $limit = PHP_INT_MAX): ArrayObject\n"
    "    {\n"
    "        return new ArrayObject(explode($delimiter, $this->string, $limit));\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param int $index\n"
    "     * @return string\n"
    "     */\n"
    "    public function char(int $index): string\n"
    "    {\n"
    "        return $this->string[$index];\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param int $chunkLength\n"
    "     * @param string $chunkEnd\n"
    "     * @return StringObject\n"
    "     */\n"
    "    public function chunkSplit(int $chunkLength = 1, string $chunkEnd = ''): StringObject\n"
    "    {\n"
    "        return new static(chunk_split($this->string, $chunkLength, $chunkEnd));\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param int $splitLength\n"
    "     * @return ArrayObject\n"
    "     */\n"
    "    public function chunk($splitLength = 1)\n"
    "    {\n"
    "        return new ArrayObject(str_split($this->string, $splitLength));\n"
    "    }\n"
    "\n"
    "\n"
    "    /**\n"
    "     * @return string\n"
    "     */\n"
    "    public function __toString(): string\n"
    "    {\n"
    "        return $this->string;\n"
    "    }\n"
    "}\n";

static const char* swoole_library_source_core_array_object =
    "\n"
    "\n"
    "namespace Swoole;\n"
    "\n"
    "use ArrayAccess;\n"
    "use Countable;\n"
    "use Iterator;\n"
    "use RuntimeException;\n"
    "use Serializable;\n"
    "\n"
    "class ArrayObject implements ArrayAccess, Serializable, Countable, Iterator\n"
    "{\n"
    "    /**\n"
    "     * @var array\n"
    "     */\n"
    "    protected $array;\n"
    "\n"
    "    /**\n"
    "     * ArrayObject constructor.\n"
    "     * @param array $array\n"
    "     */\n"
    "    public function __construct(array $array = [])\n"
    "    {\n"
    "        $this->array = $array;\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @return bool\n"
    "     */\n"
    "    public function isEmpty(): bool\n"
    "    {\n"
    "        return empty($this->array);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @return int\n"
    "     */\n"
    "    public function count(): int\n"
    "    {\n"
    "        return count($this->array);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @return mixed\n"
    "     */\n"
    "    public function current()\n"
    "    {\n"
    "        return current($this->array);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @return mixed\n"
    "     */\n"
    "    public function key()\n"
    "    {\n"
    "        return key($this->array);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @return bool\n"
    "     */\n"
    "    public function valid(): bool\n"
    "    {\n"
    "        return array_key_exists($this->key(), $this->array);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @return mixed\n"
    "     */\n"
    "    public function rewind()\n"
    "    {\n"
    "        return reset($this->array);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @return mixed\n"
    "     */\n"
    "    public function next()\n"
    "    {\n"
    "        return next($this->array);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param $key\n"
    "     * @return ArrayObject|StringObject|mixed\n"
    "     */\n"
    "    public function get($key)\n"
    "    {\n"
    "        return _detectType($this->array[$key]);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param $key\n"
    "     * @param $value\n"
    "     * @return $this\n"
    "     */\n"
    "    public function set($key, $value): self\n"
    "    {\n"
    "        $this->array[$key] = $value;\n"
    "        return $this;\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param $key\n"
    "     * @return $this\n"
    "     */\n"
    "    public function delete($key): self\n"
    "    {\n"
    "        unset($this->array[$key]);\n"
    "        return $this;\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param $value\n"
    "     * @param bool $strict\n"
    "     * @param bool $loop\n"
    "     * @return $this\n"
    "     */\n"
    "    public function remove($value, bool $strict = true, bool $loop = false): self\n"
    "    {\n"
    "        do {\n"
    "            $key = $this->search($value, $strict);\n"
    "            if ($key) {\n"
    "                unset($this->array[$key]);\n"
    "            } else {\n"
    "                break;\n"
    "            }\n"
    "        } while ($loop);\n"
    "        return $this;\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @return $this\n"
    "     */\n"
    "    public function clear(): self\n"
    "    {\n"
    "        $this->array = [];\n"
    "        return $this;\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param mixed $key\n"
    "     * @return mixed|null\n"
    "     */\n"
    "    public function offsetGet($key)\n"
    "    {\n"
    "        if (!array_key_exists($key, $this->array)) {\n"
    "            return null;\n"
    "        }\n"
    "        return $this->array[$key];\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param mixed $key\n"
    "     * @param mixed $value\n"
    "     */\n"
    "    public function offsetSet($key, $value)\n"
    "    {\n"
    "        $this->array[$key] = $value;\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param mixed $key\n"
    "     */\n"
    "    public function offsetUnset($key)\n"
    "    {\n"
    "        unset($this->array[$key]);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param mixed $key\n"
    "     * @return bool\n"
    "     */\n"
    "    public function offsetExists($key)\n"
    "    {\n"
    "        return isset($this->array[$key]);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param $key\n"
    "     * @return bool\n"
    "     */\n"
    "    public function exists($key): bool\n"
    "    {\n"
    "        return array_key_exists($key, $this->array);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param $value\n"
    "     * @param bool $strict\n"
    "     * @return bool\n"
    "     */\n"
    "    public function contains($value, bool $strict = true): bool\n"
    "    {\n"
    "        return in_array($value, $this->array, $strict);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param $value\n"
    "     * @param bool $strict\n"
    "     * @return mixed\n"
    "     */\n"
    "    public function indexOf($value, bool $strict = true)\n"
    "    {\n"
    "        return $this->search($value, $strict);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param $value\n"
    "     * @param bool $strict\n"
    "     * @return mixed\n"
    "     */\n"
    "    public function lastIndexOf($value, bool $strict = true)\n"
    "    {\n"
    "        $array = $this->array;\n"
    "        for (end($array); ($currentKey = key($array)) !== null; prev($array)) {\n"
    "            $currentValue = current($array);\n"
    "            if ($currentValue == $value) {\n"
    "                if ($strict && $currentValue !== $value) {\n"
    "                    continue;\n"
    "                }\n"
    "                break;\n"
    "            }\n"
    "        }\n"
    "        return $currentKey;\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param $find\n"
    "     * @param $strict\n"
    "     * @return mixed\n"
    "     */\n"
    "    public function search($find, $strict = true)\n"
    "    {\n"
    "        return array_search($find, $this->array, $strict);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param string $str\n"
    "     * @return string\n"
    "     */\n"
    "    public function join(string $str = ''): string\n"
    "    {\n"
    "        return implode($str, $this->array);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @return string\n"
    "     */\n"
    "    public function serialize(): string\n"
    "    {\n"
    "        return serialize($this->array);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param string $str\n"
    "     * @return $this\n"
    "     */\n"
    "    public function unserialize($str): self\n"
    "    {\n"
    "        $this->array = (array)unserialize($str);\n"
    "        return $this;\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @return float|int\n"
    "     */\n"
    "    public function sum()\n"
    "    {\n"
    "        return array_sum($this->array);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @return float|int\n"
    "     */\n"
    "    public function product()\n"
    "    {\n"
    "        return array_product($this->array);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param $value\n"
    "     * @return int\n"
    "     */\n"
    "    public function push($value)\n"
    "    {\n"
    "        return array_push($this->array, $value);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param $value\n"
    "     * @return int\n"
    "     */\n"
    "    public function pushBack($value)\n"
    "    {\n"
    "        return array_unshift($this->array, $value);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param int $offset\n"
    "     * @param $value\n"
    "     * @return $this\n"
    "     */\n"
    "    public function insert(int $offset, $value): self\n"
    "    {\n"
    "        if (is_array($value) || is_object($value) || is_null($value)) {\n"
    "            $value = [$value];\n"
    "        }\n"
    "        array_splice($this->array, $offset, 0, $value);\n"
    "        return $this;\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @return mixed\n"
    "     */\n"
    "    public function pop()\n"
    "    {\n"
    "        return array_pop($this->array);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @return mixed\n"
    "     */\n"
    "    public function popFront()\n"
    "    {\n"
    "        return array_shift($this->array);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param $offset\n"
    "     * @param int $length\n"
    "     * @param bool $preserve_keys\n"
    "     * @return static\n"
    "     */\n"
    "    public function slice($offset, int $length = null, bool $preserve_keys = false): self\n"
    "    {\n"
    "        return new static(array_slice($this->array, ...func_get_args()));\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @return ArrayObject|StringObject\n"
    "     */\n"
    "    public function randomGet()\n"
    "    {\n"
    "        return _detectType($this->array[array_rand($this->array, 1)]);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param $fn callable\n"
    "     * @return $this\n"
    "     */\n"
    "    public function each(callable $fn): self\n"
    "    {\n"
    "        if (array_walk($this->array, $fn) === false) {\n"
    "            throw new RuntimeException('array_walk() failed');\n"
    "        }\n"
    "        return $this;\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param $fn callable\n"
    "     * @return static\n"
    "     */\n"
    "    public function map(callable $fn): self\n"
    "    {\n"
    "        return new static(array_map($fn, $this->array));\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param $fn callable\n"
    "     * @return mixed\n"
    "     */\n"
    "    public function reduce(callable $fn)\n"
    "    {\n"
    "        return array_reduce($this->array, $fn);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param int $search_value\n"
    "     * @param bool $strict\n"
    "     * @return static\n"
    "     */\n"
    "    public function keys(int $search_value = null, $strict = false): self\n"
    "    {\n"
    "        return new static(array_keys($this->array, $search_value, $strict));\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @return static\n"
    "     */\n"
    "    public function values(): self\n"
    "    {\n"
    "        return new static(array_values($this->array));\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param $column_key\n"
    "     * @param mixed ...$index\n"
    "     * @return static\n"
    "     */\n"
    "    public function column($column_key, ...$index): self\n"
    "    {\n"
    "        return new static(array_column($this->array, $column_key, ...$index));\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param int $sort_flags\n"
    "     * @return static\n"
    "     */\n"
    "    public function unique(int $sort_flags = SORT_STRING): self\n"
    "    {\n"
    "        return new static(array_unique($this->array, $sort_flags));\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param bool $preserve_keys\n"
    "     * @return static\n"
    "     */\n"
    "    public function reverse(bool $preserve_keys = false): self\n"
    "    {\n"
    "        return new static(array_reverse($this->array, $preserve_keys));\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param int $size\n"
    "     * @param bool $preserve_keys\n"
    "     * @return static\n"
    "     */\n"
    "    public function chunk(int $size, bool $preserve_keys = false): self\n"
    "    {\n"
    "        return new static(array_chunk($this->array, $size, $preserve_keys));\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * Swap keys and values in an array\n"
    "     * @return static\n"
    "     */\n"
    "    public function flip(): self\n"
    "    {\n"
    "        return new static(array_flip($this->array));\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param $fn callable\n"
    "     * @param int $flag\n"
    "     * @return static\n"
    "     */\n"
    "    public function filter(callable $fn, int $flag = 0): self\n"
    "    {\n"
    "        return new static(array_filter($this->array, $fn, $flag));\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * | Function name     | Sorts by | Maintains key association   | Order of sort               | Related functions |\n"
    "     * | :---------------- | :------- | :-------------------------- | :-------------------------- | :---------------- |\n"
    "     * | array_multisort() | value    | associative yes, numeric no | first array or sort options | array_walk()      |\n"
    "     * | asort()           | value    | yes                         | low to high                 | arsort()          |\n"
    "     * | arsort()          | value    | yes                         | high to low                 | asort()           |\n"
    "     * | krsort()          | key      | yes                         | high to low                 | ksort()           |\n"
    "     * | ksort()           | key      | yes                         | low to high                 | asort()           |\n"
    "     * | natcasesort()     | value    | yes                         | natural, case insensitive   | natsort()         |\n"
    "     * | natsort()         | value    | yes                         | natural                     | natcasesort()     |\n"
    "     * | rsort()           | value    | no                          | high to low                 | sort()            |\n"
    "     * | shuffle()         | value    | no                          | random                      | array_rand()      |\n"
    "     * | sort()            | value    | no                          | low to high                 | rsort()           |\n"
    "     * | uasort()          | value    | yes                         | user defined                | uksort()          |\n"
    "     * | uksort()          | key      | yes                         | user defined                | uasort()          |\n"
    "     * | usort()           | value    | no                          | user defined                | uasort()          |\n"
    "     */\n"
    "\n"
    "    /**\n"
    "     * @param int $sort_order\n"
    "     * @param int $sort_flags\n"
    "     * @return $this\n"
    "     */\n"
    "    public function multiSort(int $sort_order = SORT_ASC, int $sort_flags = SORT_REGULAR): self\n"
    "    {\n"
    "        if (array_multisort($this->array, $sort_order, $sort_flags) !== true) {\n"
    "            throw new RuntimeException('array_multisort() failed');\n"
    "        }\n"
    "        return $this;\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param int $sort_flags\n"
    "     * @return $this\n"
    "     */\n"
    "    public function asort(int $sort_flags = SORT_REGULAR): self\n"
    "    {\n"
    "        if (asort($this->array, $sort_flags) !== true) {\n"
    "            throw new RuntimeException('asort() failed');\n"
    "        }\n"
    "        return $this;\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param int $sort_flags\n"
    "     * @return $this\n"
    "     */\n"
    "    public function arsort(int $sort_flags = SORT_REGULAR): self\n"
    "    {\n"
    "        if (arsort($this->array, $sort_flags) !== true) {\n"
    "            throw new RuntimeException('arsort() failed');\n"
    "        }\n"
    "        return $this;\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param int $sort_flags\n"
    "     * @return $this\n"
    "     */\n"
    "    public function krsort(int $sort_flags = SORT_REGULAR): self\n"
    "    {\n"
    "        if (krsort($this->array, $sort_flags) !== true) {\n"
    "            throw new RuntimeException('krsort() failed');\n"
    "        }\n"
    "        return $this;\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param int $sort_flags\n"
    "     * @return $this\n"
    "     */\n"
    "    public function ksort(int $sort_flags = SORT_REGULAR): self\n"
    "    {\n"
    "        if (ksort($this->array, $sort_flags) !== true) {\n"
    "            throw new RuntimeException('ksort() failed');\n"
    "        }\n"
    "        return $this;\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @return $this\n"
    "     */\n"
    "    public function natcasesort(): self\n"
    "    {\n"
    "        if (natcasesort($this->array) !== true) {\n"
    "            throw new RuntimeException('natcasesort() failed');\n"
    "        }\n"
    "        return $this;\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @return $this\n"
    "     */\n"
    "    public function natsort(): self\n"
    "    {\n"
    "        if (natsort($this->array) !== true) {\n"
    "            throw new RuntimeException('natsort() failed');\n"
    "        }\n"
    "        return $this;\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param int $sort_flags\n"
    "     * @return $this\n"
    "     */\n"
    "    public function rsort(int $sort_flags = SORT_REGULAR): self\n"
    "    {\n"
    "        if (rsort($this->array, $sort_flags) !== true) {\n"
    "            throw new RuntimeException('rsort() failed');\n"
    "        }\n"
    "        return $this;\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @return $this\n"
    "     */\n"
    "    public function shuffle(): self\n"
    "    {\n"
    "        if (shuffle($this->array) !== true) {\n"
    "            throw new RuntimeException('shuffle() failed');\n"
    "        }\n"
    "        return $this;\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param int $sort_flags\n"
    "     * @return $this\n"
    "     */\n"
    "    public function sort(int $sort_flags = SORT_REGULAR): self\n"
    "    {\n"
    "        if (sort($this->array, $sort_flags) !== true) {\n"
    "            throw new RuntimeException('sort() failed');\n"
    "        }\n"
    "        return $this;\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param callable $value_compare_func\n"
    "     * @return $this\n"
    "     */\n"
    "    public function uasort(callable $value_compare_func): self\n"
    "    {\n"
    "        if (uasort($this->array, $value_compare_func) !== true) {\n"
    "            throw new RuntimeException('uasort() failed');\n"
    "        }\n"
    "        return $this;\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param callable $value_compare_func\n"
    "     * @return $this\n"
    "     */\n"
    "    public function uksort(callable $value_compare_func): self\n"
    "    {\n"
    "        if (uksort($this->array, $value_compare_func) !== true) {\n"
    "            throw new RuntimeException('uksort() failed');\n"
    "        }\n"
    "        return $this;\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param callable $value_compare_func\n"
    "     * @return $this\n"
    "     */\n"
    "    public function usort(callable $value_compare_func): self\n"
    "    {\n"
    "        if (usort($this->array, $value_compare_func) !== true) {\n"
    "            throw new RuntimeException('usort() failed');\n"
    "        }\n"
    "        return $this;\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @return array\n"
    "     */\n"
    "    public function __toArray(): array\n"
    "    {\n"
    "        return $this->array;\n"
    "    }\n"
    "\n"
    "}\n";

static const char* swoole_library_source_functions =
    "\n"
    "if (ini_get('swoole.use_shortname') === 'On') {\n"
    "    /**\n"
    "     * @param string $string\n"
    "     * @return \\Swoole\\StringObject\n"
    "     */\n"
    "    function _string(string $string = ''): Swoole\\StringObject\n"
    "    {\n"
    "        return new Swoole\\StringObject($string);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param array $array\n"
    "     * @return \\Swoole\\ArrayObject\n"
    "     */\n"
    "    function _array(array $array = []): Swoole\\ArrayObject\n"
    "    {\n"
    "        return new Swoole\\ArrayObject($array);\n"
    "    }\n"
    "\n"
    "    /**\n"
    "     * @param $value\n"
    "     * @return \\Swoole\\StringObject|\\Swoole\\ArrayObject\n"
    "     */\n"
    "    function _detectType($value)\n"
    "    {\n"
    "        if (is_array($value)) {\n"
    "            return new \\Swoole\\ArrayObject($value);\n"
    "        } elseif (is_string($value)) {\n"
    "            return new \\Swoole\\StringObject($value);\n"
    "        } else {\n"
    "            return $value;\n"
    "        }\n"
    "    }\n"
    "}\n";

static const char* swoole_library_source_alias =
    "\n"
    "if (ini_get('swoole.use_shortname') === 'On') {\n"
    "    class_alias(Swoole\\Coroutine\\WaitGroup::class, Co\\WaitGroup::class, false);\n"
    "}\n";

static void php_swoole_load_library()
{
    zend::eval(swoole_library_source_constants, "@swoole-src/library/constants.php");
    zend::eval(swoole_library_source_std_array, "@swoole-src/library/std/array.php");
    zend::eval(swoole_library_source_ext_curl, "@swoole-src/library/ext/curl.php");
    zend::eval(swoole_library_source_core_coroutine_wait_group, "@swoole-src/library/core/Coroutine/WaitGroup.php");
    zend::eval(swoole_library_source_core_string_object, "@swoole-src/library/core/StringObject.php");
    zend::eval(swoole_library_source_core_array_object, "@swoole-src/library/core/ArrayObject.php");
    zend::eval(swoole_library_source_functions, "@swoole-src/library/functions.php");
    zend::eval(swoole_library_source_alias, "@swoole-src/library/alias.php");
}
